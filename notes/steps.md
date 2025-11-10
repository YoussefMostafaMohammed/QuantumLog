# Jenkins + Docker + GitHub CI/CD Setup Guide: A Deep Dive into Automation, Integration, and Troubleshooting

## Introduction

This comprehensive guide walks you through setting up **Jenkins** (an open-source automation server) to automatically build, test, and run code from a GitHub repository upon every push, leveraging **Docker** for containerized builds.

- **Flow Overview**: GitHub push → Webhook notification → Jenkins receives trigger via exposed endpoint (e.g., ngrok) → Pipeline execution on Jenkins node → Git checkout → Docker build/run → Post-actions (success/failure handling).

- **Integrations**: Jenkins integrates with GitHub via webhooks and SCM plugins; Docker via CLI commands (or emulations like Podman); Plugins extend Jenkins for pipelines.

We will cover:
1. Installing Jenkins standalone (mechanics of service setup and initial unlock).
2. Installing Docker (full steps, including group permissions and potential Podman emulation pitfalls).
3. Installing the Pipeline plugin (how it enables declarative scripting).
4. Using ngrok to expose Jenkins (tunneling mechanics for local-to-public integration).
5. Integrating Jenkins with GitHub (webhook flows and authentication).
6. Writing a full Jenkins pipeline for Dockerized builds (Groovy syntax, stage flows, and error handling).
7. Detailed common issues, including your specific errors (deep analysis of rootless containers, namespaces, and systemd integrations).
8. Testing and notes.

---

## Step 1: Install Jenkins (Standalone) – Understanding the Automation Server Setup

Jenkins runs as a Java-based service, listening on port 8080 by default. The installation flow integrates with your system's package manager (apt) and systemd for daemon management.

1. **Update Packages and Install Prerequisites**:
   - Flow: `apt update` refreshes repository indexes → Install Java (required for Jenkins) and tools like wget/gnupg2 for key management.
   ```bash
    sudo apt update
    sudo apt install -y openjdk-11-jdk wget gnupg2
   ```

2. **Add Jenkins Repository and Key**:
   - Flow: Download key → Add to apt keyring → Create repo list → Update indexes.
   ```bash
   wget -q -O - https://pkg.jenkins.io/debian-stable/jenkins.io.key | sudo apt-key add -
   sudo sh -c 'echo deb https://pkg.jenkins.io/debian-stable binary/ > /etc/apt/sources.list.d/jenkins.list'
   sudo apt update
   ```

3. **Install Jenkins**:
   ```bash
   sudo apt install -y jenkins
   ```
    - Post-Install Flow: Apt installs binaries, configs (/etc/default/jenkins), and starts the service via systemd.

4. **Start and Enable Jenkins**:
   ```bash
   sudo systemctl start jenkins
   sudo systemctl enable jenkins
   sudo systemctl status jenkins
   ```
   - How It Works: Systemd manages the daemon (jenkins.service). `start` launches it; `enable` auto-starts on boot. `status` shows logs/PID.
   - Integration: Jenkins now runs as user 'jenkins' in /var/lib/jenkins.

5. **Access Jenkins Web UI**:
   - Open `http://localhost:8080`.
   - Flow: Browser → Jenkins servlet (Tomcat-like) → Initial setup page.

6. **Unlock Jenkins**:
   ```bash
    sudo cat /var/lib/jenkins/secrets/initialAdminPassword
   ```

7. **Complete Setup**:
   - Install suggested plugins (e.g., Git, Pipeline basics).
   - Create admin user.
   - Flow: Plugins extend Jenkins' core; user setup integrates with role-based access control (RBAC).

**Deep Note**: Jenkins' architecture: Master (controller) orchestrates agents (nodes). Here, it's standalone (master as agent). Plugins like Pipeline add DSL for scripting flows.

---

## Step 2: Install Docker (Full Steps) – Container Runtime Mechanics and Podman Emulation

Docker enables containerization: Lightweight VMs with isolated filesystems, networks, and processes. Installation flow: Remove conflicts → Add repo/key → Install → Configure groups. **Important**: If your system uses Podman (daemonless alternative) to emulate Docker CLI (common on RHEL/Fedora or if Docker isn't fully installed), additional rootless mode configs are needed— we'll cover your errors here.

**Your First Error: User Namespace Issues in Podman-Emulated Docker**
-  Assign UID/GID ranges to 'jenkins' (e.g., 100000:65536). This allows mapping diverse ownerships.

**Your Second Error: sd-bus Permission Denied in Crun Runtime**
-Enable lingering (persistent session) and install D-Bus tools.

1. **Install Prerequisites**:
   ```bash
   sudo apt update
   sudo apt install -y apt-transport-https ca-certificates curl software-properties-common gnupg lsb-release
   ```

2. **Add Docker’s GPG Key**:
   ```bash
   curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
   ```

3. **Add Repository**:
   ```bash
   echo \
     "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] \
     https://download.docker.com/linux/ubuntu \
     $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
   ```

4. **Update and Install Docker**:
   ```bash
   sudo apt update
   sudo apt install -y docker-ce docker-ce-cli containerd.io
   ```

5. **Start and Enable Docker**:
   ```bash
   sudo systemctl start docker
   sudo systemctl enable docker
   sudo systemctl status docker
   ```
   - Flow: Systemd starts dockerd (daemon) as root, listening on /var/run/docker.sock.

6. **Add Users to Docker Group** (Fix Permission Denied):
   ```bash
   sudo usermod -aG docker $USER
   sudo usermod -aG docker jenkins
   ```
   - Why? Allows non-root access to Docker socket. Restart session/PC to apply.

7. **Podman-Specific Fixes (If Emulating Docker)**:
   - Check: Run `docker --version` – if mentions Podman, apply these.
   
   - **For Namespace Error**:
     ```bash
     echo "jenkins:100000:65536" | sudo tee -a /etc/subuid /etc/subgid
     sudo systemctl restart jenkins
     ```
     - Flow: Files define mappable IDs → Podman uses for namespaces → Successful chown.
   
   - **For sd-bus Error**:
     ```bash
     sudo loginctl enable-linger jenkins  # Or UID, e.g., 135
     sudo apt install -y dbus dbus-x11
     sudo systemctl restart jenkins
     ```
   
**Build/Run Example**:
```bash
docker build -t quantumlog-build:latest .
docker run --rm -it quantumlog-build:latest
```
- Integration: Build creates image layers; Run spawns container with binds/mounts.

---

## Step 3: Install Jenkins Pipeline Plugin – Enabling Declarative Pipelines

The Pipeline plugin allows scripting CI/CD as code (Jenkinsfile in repo).

1. In Jenkins UI: **Manage Jenkins → Manage Plugins → Available**.
2. Search "Pipeline", install (includes dependencies like Workflow).
3. Restart if prompted.
- **How It Works**: Plugin adds Groovy DSL for stages, steps, agents. Flow: Jenkins parses Jenkinsfile → Executes on agent → Integrates with tools like sh, git.

---

## Step 4: Install ngrok – Tunneling for Local Exposure

Ngrok creates secure tunnels from public URLs to local ports, bypassing NAT/firewalls for webhook testing.

**Problem**: Local Jenkins isn't internet-accessible for GitHub webhooks.
- **Solution**: Ngrok proxies requests. Flow: GitHub POST → Ngrok server → Tunnel to localhost:8080.

1. **Download and Install**:
   ```bash
   wget https://bin.equinox.io/c/bNyj1mQVY4c/ngrok-v3-stable-linux-amd64.tgz
   tar -xvzf ngrok-v3-stable-linux-amd64.tgz
   sudo mv ngrok /usr/local/bin/
   ```

2. **Authenticate** (Fix Auth Error):
   - Visit https://dashboard.ngrok.com/get-started/your-authtoken.
   ```bash
   ngrok config add-authtoken YOUR_AUTHTOKEN
   ```

3. **Start Ngrok**:
   ```bash
   ngrok http 8080
   ```
   - Output: Forwarding https://random.ngrok.io → http://localhost:8080.

**GitHub Webhook Setup Example**:
1. Repo Settings → Webhooks → Add webhook.
2. Payload URL: https://random.ngrok.io/github-webhook/ (Jenkins endpoint).
3. Content: application/json; Events: Push.
- **Flow**: Push → GitHub sends JSON payload → Ngrok → Jenkins triggers pipeline.
- **Verification**: Check Jenkins logs for incoming requests.

**Tip**: Free ngrok changes URLs on restart; paid plans reserve domains.

---

## Step 5: Connect Jenkins to GitHub – Webhook and SCM Integration

1. **Configure GitHub in Jenkins**: Manage Jenkins → Configure System → GitHub → Add server with Personal Access Token (scopes: repo, admin:repo_hook).
   - Integration: Token authenticates pulls; Webhooks trigger builds.

2. **Create Pipeline Job**: New Item → Pipeline → Pipeline script from SCM → Git → Repo URL, Branch (Phase_01).
   - Flow: Job polls or webhook → Git plugin fetches Jenkinsfile → Executes.

3. **Add Webhook in GitHub**: Settings → Webhooks → Add → Payload URL (ngrok + /github-webhook/), Events: Push.
   - Deep Flow: Commit push → GitHub event → HTTP POST to webhook → Jenkins processes (via GitHub plugin) → Pipeline start.

---

## Step 6: Jenkins Pipeline for Docker Build – Full Script with Explanations

Place this `Jenkinsfile` in your repo root. It's declarative Groovy: Defines agents, env vars, stages (sequential flow), and post-actions.

```groovy
pipeline {
    agent any  // Runs on any available node (here, master).

    environment {  // Global vars; integrate with steps via ${VAR}.
        CXX_COMPILER = "/usr/bin/g++"
        DOCKER_IMAGE = "quantumlog-build:latest"
    }

    stages {
        stage('Checkout') {  // Fetches code; integrates with Git plugin.
            steps {
                git branch: 'Phase_01', url: 'https://github.com/YoussefMostafaMohammed/QuantumLog.git'
            }
        }

        stage('Build Docker Image') {  // Builds image; sh calls Docker/Podman.
            steps {
                script {  // For complex logic.
                    sh """
                    docker build -t ${DOCKER_IMAGE} .
                    """
                }
            }
        }

        stage('Build Project inside Docker') {  // Runs build in container; -v mounts workspace.
            steps {
                script {
                    sh """
                    docker run --rm -v \$(pwd):/workspace -w /workspace ${DOCKER_IMAGE} /bin/bash -c \"
                        rm -rf build &&  // Clears cache to avoid mismatches.
                        mkdir -p build &&
                        cd build &&
                        cmake .. -DCMAKE_CXX_COMPILER=${CXX_COMPILER} -DCMAKE_BUILD_TYPE=Debug &&
                        make -j\$(nproc) &&
                        ./QuantumLog
                    \"
                    """
                }
            }
        }

        stage('Optional: Run Tests') {  // Conditional; fileExists checks output.
            when {
                expression { fileExists('build/CTestTestfile.cmake') }
            }
            steps {
                sh """
                docker run --rm -v \$(pwd):/workspace -w /workspace ${DOCKER_IMAGE} ctest --output-on-failure
                """
            }
        }
    }

    post {  // Always runs after stages.
        success {
            echo 'Build completed successfully!'
        }
        failure {
            echo 'Build failed!'
        }
    }
}
```

**Your CMake Error Fix**: `rm -rf build` prevents cache mismatches (e.g., CMakeCache.txt from prior runs). Flow: Container starts → Clean dir → cmake generates new config → make compiles.

**Deep Flow**: Trigger → Agent allocation → Env setup → Stage 1 (git fetch) → Stage 2 (image build) → Stage 3 (mount & run build) → Optional tests → Post-echo.

---

## Step 7: Common Issues & Solutions – Detailed Analysis for Learning

| Problem | Deep Explanation & Flow | Solution & Integration |
|---------|--------------------------|------------------------|
| Docker commands fail with `permission denied` | Docker daemon requires socket access; non-root users need group. Flow: CLI → Socket → Denied if not in group. | Add to docker group: `sudo usermod -aG docker jenkins`; Restart session. Integrates with Unix permissions. |
| CMake cache errors inside Docker (e.g., mismatch in CMakeCache.txt) | Cached build dir retains old configs; Docker volumes persist them. Flow: cmake → Checks cache → Mismatch if paths/compilers differ. | Always `rm -rf build` before cmake. Teaches build hygiene in containers. |
| Jenkins cannot reach GitHub or vice versa | Local firewall/NAT blocks; No public IP. Flow: Push → Webhook fails. | Use ngrok for tunneling. Integrates HTTP proxying. |
| Jenkins cannot run Docker inside (Podman emulation: Insufficient UIDs/GIDs) | As above: Rootless Podman needs subuid for namespaces. | Assign ranges: `echo "jenkins:100000:65536" | sudo tee -a /etc/subuid /etc/subgid`; Restart Jenkins. |
| Podman/Crun: sd-bus call Permission Denied | As above: No user session for D-Bus/systemd integration in rootless mode. | Enable linger: `sudo loginctl enable-linger jenkins`; Install dbus: `sudo apt install dbus dbus-x11`; Restart. |
| Cgroup warnings in Podman | No session → Fallback from cgroupv2 to v1. Flow: Podman → Systemd check → Warn. | Same as above; Lingering enables proper cgroup management. |

**Study Tip**: These errors highlight rootless security (no root daemon) vs. usability trade-offs. Read man user_namespaces, podman-rootless-tutorial.

---

## Step 8: Test Your Setup – Validation Flow

1. Commit/push to Phase_01.
2. Monitor Jenkins: Webhook → Pipeline start → Logs show stages.
3. Verify: Image built, project runs (output in logs).
- **Flow**: End-to-end test ensures integrations (GitHub → Jenkins → Docker).

---

## Notes

- Tested on Ubuntu 22.04 LTS (2025 updates may vary; check repos).
- Restart after group changes: Applies getgroups() updates.
- Ngrok for dev only; Production: Use VPS with static IP, HTTPS.
- Security: Use tokens sparingly; Rotate if exposed.
- Extensions: Add email notifications in post{} for real CI/CD.

---

## References

- [Jenkins Official](https://www.jenkins.io/)
- [Docker Official](https://docs.docker.com/engine/install/ubuntu/)
- [Podman Docs](https://podman.io/docs/tutorials/rootless_tutorial)
- [ngrok](https://ngrok.com/)
- [Systemd User Sessions](https://systemd.io/USER_NAMESPACES/)
