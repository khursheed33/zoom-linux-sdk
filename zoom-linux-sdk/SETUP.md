# Zoom Meeting SDK Linux Sample App Setup Guide

This guide walks you through setting up and running the `zoom-linux-sdk` sample app on an Ubuntu system. It assumes you’re starting fresh after cloning the repository.

## Prerequisites
- **Ubuntu system** (tested on 22.04/23.04)  
- A Zoom account and a [Zoom Meeting SDK App](https://marketplace.zoom.us) with Client ID and Secret  
- Git, CMake, and basic build tools installed  
- A valid Zoom meeting number and (optional) password  

## Step-by-Step Setup

### 1. Install System Dependencies
Update your package list and install required tools and libraries:
```bash
sudo apt update && sudo apt install -y git cmake g++ build-essential libcurl4-openssl-dev
```

---

### 2. Clone the Repository
Clone the `zoom-linux-sdk` repository to your system:
```bash
git clone https://github.com/khursheed33/zoom-linux-sdk.git
cd zoom-linux-sdk
```

---

### 3. (Optional) If Using `docker-setup` Branch  
If you are using the `docker-setup` branch, you **do not need to manually copy and paste the SDK files**. The `docker-setup` branch uses SDK version **6.1.10** and handles SDK integration automatically.  

Switch to the `docker-setup` branch:
```bash
git checkout docker-setup
```

---

### 4. Download and Integrate Zoom Meeting SDK (If Not Using Docker)  
1. Download the Linux Meeting SDK from [marketplace.zoom.us](https://marketplace.zoom.us) (e.g., `zoom-meeting-sdk-linux_x86_64-5.xx.x.xxxx.tar`).  
2. Extract it:
   ```bash
   tar -xvf ~/Downloads/zoom-meeting-sdk-linux_x86_64-5.xx.x.xxxx.tar -C ~/Downloads/
   ```
3. Copy the SDK files to the `demo/` directory:
   ```bash
   cp -r ~/Downloads/zoom-meeting-sdk-linux_x86_64-5.xx.x.xxxx/h demo/include/
   cp -r ~/Downloads/zoom-meeting-sdk-linux_x86_64-5.xx.x.xxxx/qt_libs demo/lib/zoom_meeting_sdk/
   cp ~/Downloads/zoom-meeting-sdk-linux_x86_64-5.xx.x.xxxx/lib*.so demo/lib/zoom_meeting_sdk/
   mkdir -p demo/lib/zoom_meeting_sdk/json
   cp ~/Downloads/zoom-meeting-sdk-linux_x86_64-5.xx.x.xxxx/translation.json demo/lib/zoom_meeting_sdk/json/
   cd demo/lib/zoom_meeting_sdk/
   ln -s libmeetingsdk.so libmeetingsdk.so.1
   cd -
   ```
   ✅ Replace `~/Downloads/zoom-meeting-sdk-linux_x86_64-5.xx.x.xxxx` with your actual extracted folder path.

---

### 5. Build the Project  
Generate build files and compile the demo:
```bash
cd demo
cmake -B build
cd build
make
```
After this, the executable `meetingSDKDemo` will be in `demo/bin/`.

---

### 6. (Optional) Build and Run Using Docker  
If you want to build and run the project using Docker (uses SDK version **6.0.2**):  

1. Build the Docker image:
```bash
docker build -t msdk-6.0.2-on-ubuntu-compact -f Dockerfile-Ubuntu/Dockerfile .
```

2. Run the container:
```bash
docker run -it --rm msdk-6.0.2-on-ubuntu-compact
```

---

### 7. Generate a JWT Token  
The demo requires a JWT token for authentication. Use the provided `GenerateSDKJwtToken.cpp` (or create your own):

1. **Edit the File**:  
   Open `GenerateSDKJwtToken.cpp` (place it in the `demo/` directory if it’s not there):
   ```bash
   nano GenerateSDKJwtToken.cpp
   ```
   Update the `main()` function with your credentials:
   ```cpp
   std::string sdk_key = "your_client_id_here";    // From Zoom Marketplace
   std::string sdk_secret = "your_client_secret_here"; // From Zoom Marketplace
   std::string meeting_number = "1234567890";      // Your meeting number
   int role = 0;                                   // 0 = participant, 1 = host
   ```

2. **Compile It**:
   ```bash
   g++ -o generate_jwt GenerateSDKJwtToken.cpp -std=c++11
   ```

3. **Run It**:
   ```bash
   ./generate_jwt
   ```
   Copy the output token (e.g., `xxxxx.yyyyy.zzzzz`).

---

### 8. Configure the Demo  
Edit the `config.txt` file in `demo/`:
```bash
nano config.txt
```
Add your details:
```
meeting_number: "1234567890"         # Your meeting number
token: "xxxxx.yyyyy.zzzzz"           # From ./generate_jwt
meeting_password: "your_password"    # Meeting password (if any)
recording_token: ""                  # Optional
GetVideoRawData: "true"
GetAudioRawData: "true"
SendVideoRawData: "false"
SendAudioRawData: "false"
```

---

### 9. Run the Demo  
Execute the compiled demo:
```bash
cd bin
./meetingSDKDemo
```
✅ If successful, it will authenticate and join the specified meeting. Ensure the meeting is active and you have appropriate permissions.

---

### 10. Clean Build (Optional)  
If you want to clean up and rebuild:
```bash
cd demo/build
make clean
cmake ..
make
```

---

### 11. Stop Running Docker Container  
If running the demo in Docker, stop the container using:
```bash
docker ps  # List running containers
docker stop <container_id>
```
Or if running in detached mode:
```bash
docker kill <container_id>
```

---

## Troubleshooting  
| Issue | Solution |
|-------|----------|
| **Error: `cannot find -lcurl`** | Install `libcurl4-openssl-dev` (Step 1). |
| **Authentication Failure** | Verify your JWT token, meeting number, and that the meeting exists. |
| **Raw Data Access Failure** | You need host/co-host rights or a recording token for raw audio/video. |
| **Docker Build Error** | Ensure Docker is installed and the Dockerfile path is correct. |

---

## Notes  
✅ Replace example values (e.g., `1234567890`) with your actual meeting details.  
✅ The JWT token expires (default: 2 hours); regenerate it as needed.  
✅ The `docker-setup` branch simplifies setup using Docker and SDK version **6.1.10**.  
✅ Refer to the original [README.md](README.md) for additional details.  

