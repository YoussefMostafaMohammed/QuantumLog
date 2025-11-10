# Jenkins + Docker + GitHub CI/CD Setup Guide

## Introduction

This guide explains how to set up **Jenkins** to build a GitHub repository automatically on each push using **Docker**. Jenkins is a popular open-source automation server that can build, test, and deploy applications. This guide includes all installation steps, common problems faced, and their solutions, making it a **complete end-to-end setup**.

We will cover:
1. Installing Jenkins without Docker
2. Installing Docker (full steps)
3. Installing Pipeline plugin in Jenkins
4. Installing ngrok to expose Jenkins to the web
5. Integrating Jenkins with GitHub
6. Writing a full Jenkins pipeline for building a Dockerized project
7. Handling common issues

---

## Step 1: Install Jenkins (Standalone)

1. Update packages:

```bash
sudo apt update
sudo apt install -y openjdk-11-jdk wget gnupg2
````

2. Add Jenkins repository and key:

```bash
wget -q -O - https://pkg.jenkins.io/debian-stable/jenkins.io.key | sudo apt-key add -
sudo sh -c 'echo deb https://pkg.jenkins.io/debian-stable binary/ > /etc/apt/sources.list.d/jenkins.list'
sudo apt update
```

3. Install Jenkins:

```bash
sudo apt install -y jenkins
```

4. Start Jenkins:

```bash
sudo systemctl start jenkins
sudo systemctl enable jenkins
sudo systemctl status jenkins
```

5. Open Jenkins in browser:

```
http://localhost:8080
```

6. Unlock Jenkins using:

```bash
sudo cat /var/lib/jenkins/secrets/initialAdminPassword
```

7. Install suggested plugins and create your first admin user.

---

## Step 2: Install Docker (Full Steps)

**Problem you faced:** Jenkins pipeline failed with `permission denied` or Docker commands failed.

**Solution:** Proper Docker installation and adding users to the Docker group.

1. Remove old versions:

```bash
sudo apt remove docker docker-engine docker.io containerd runc -y
```

2. Install prerequisites:

```bash
sudo apt update
sudo apt install -y apt-transport-https ca-certificates curl software-properties-common gnupg lsb-release
```

3. Add Docker’s official GPG key:

```bash
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
```

4. Add Docker repository:

```bash
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] \
  https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
```

5. Update package index:

```bash
sudo apt update
```

6. Install Docker:

```bash
sudo apt install -y docker-ce docker-ce-cli containerd.io
```

7. Start and enable Docker service:

```bash
sudo systemctl start docker
sudo systemctl enable docker
sudo systemctl status docker
```

8. Add users to Docker group:

```bash
sudo usermod -aG docker $USER
sudo usermod -aG docker jenkins
```

> **Important:** Log out and log back in (or restart your PC) to apply group changes.

9. Test Docker:

```bash
docker run hello-world
```

---

## Step 3: Install Jenkins Pipeline Plugin

1. Go to **Manage Jenkins → Manage Plugins → Available**.
2. Search for **Pipeline** and install it.
3. Restart Jenkins if required.

---

## Step 4: Install ngrok (Expose Jenkins to the Internet)

**Problem:** You want to connect GitHub webhooks to a local Jenkins server.

1. Download ngrok:

```bash
wget https://bin.equinox.io/c/4VmDzA7iaHb/ngrok-stable-linux-amd64.zip
unzip ngrok-stable-linux-amd64.zip
sudo mv ngrok /usr/local/bin
```

2. Start ngrok to expose Jenkins:

```bash
ngrok http 8080
```

3. Copy the public URL provided by ngrok. Use it in GitHub webhook.

---

## Step 5: Connect Jenkins to GitHub

1. Go to **Manage Jenkins → Configure System → GitHub**.
2. Add **GitHub server** with your personal access token.
3. Create a **new Pipeline Job**.
4. Select **Pipeline from SCM → Git → URL of your repo**.
5. Select branch (e.g., `Phase_01`).
6. Add webhook in GitHub: **Settings → Webhooks → Add webhook** with your ngrok URL and `/github-webhook/`.

---

## Step 6: Jenkins Pipeline for Docker Build

Create a file `Jenkinsfile` in your repository:

```groovy
pipeline {
    agent any

    environment {
        CXX_COMPILER = "/usr/bin/g++"
        DOCKER_IMAGE = "quantumlog-build:latest"
    }

    stages {
        stage('Checkout') {
            steps {
                git branch: 'Phase_01', url: 'https://github.com/YoussefMostafaMohammed/QuantumLog.git'
            }
        }

        stage('Build Docker Image') {
            steps {
                script {
                    sh """
                    docker build -t ${DOCKER_IMAGE} .
                    """
                }
            }
        }

        stage('Build Project inside Docker') {
            steps {
                script {
                    sh """
                    docker run --rm -v \$(pwd):/workspace -w /workspace ${DOCKER_IMAGE} /bin/bash -c \"
                        rm -rf build &&
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

        stage('Optional: Run Tests') {
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

    post {
        success {
            echo 'Build completed successfully!'
        }
        failure {
            echo 'Build failed!'
        }
    }
}
```

**Problem solved here:**

* Previous CMake errors (`CMakeCache.txt mismatch`) were due to using cached build folders inside Docker. The pipeline now **removes old build folder** before building inside Docker.

---

## Step 7: Common Issues & Solutions

| Problem                                       | Solution                                                            |
| --------------------------------------------- | ------------------------------------------------------------------- |
| Docker commands fail with `permission denied` | Add user (and `jenkins`) to Docker group, restart PC/session        |
| CMake cache errors inside Docker              | Always remove `build` folder before running `cmake`                 |
| Jenkins cannot reach GitHub                   | Use ngrok to expose local Jenkins and configure webhook             |
| Jenkins cannot run Docker inside Docker       | Ensure Jenkins user is in Docker group and Docker daemon is running |

---

## Step 8: Test Your Setup

1. Push a commit to GitHub (`Phase_01` branch).
2. Check Jenkins pipeline triggered automatically.
3. Verify Docker image builds and project runs successfully.

---

## Notes

* This setup is tested on **Ubuntu 22.04 LTS**.
* Make sure you restart PC or log out after adding users to the Docker group.
* Use **ngrok** only for testing webhooks. For production, use a server with a public IP.

---

## References

* [Jenkins Official](https://www.jenkins.io/)
* [Docker Official](https://docs.docker.com/engine/install/ubuntu/)
* [ngrok](https://ngrok.com/)
