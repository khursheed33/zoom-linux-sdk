# Zoom Meeting SDK Linux Sample App Setup Guide

This guide walks you through setting up and running the `zoom-linux-sdk` sample app on an Ubuntu system. It assumes you’re starting fresh after cloning the repository.

## Prerequisites
- Ubuntu system (tested on 22.04/23.04)
- A Zoom account and a [Zoom Meeting SDK App](https://marketplace.zoom.us) with Client ID and Secret
- Git, CMake, and basic build tools installed
- A valid Zoom meeting number and (optional) password

## Step-by-Step Setup

### 1. Install System Dependencies
Update your package list and install required tools and libraries:
```bash
sudo apt update && sudo apt install -y git cmake g++ build-essential libcurl4-openssl-dev
```

### 2. Clone the Repository
Clone the `zoom-linux-sdk` repository to your system:
```bash
git clone https://github.com/khursheed33/zoom-linux-sdk.git
cd zoom-linux-sdk
```

### 3. Download and Integrate Zoom Meeting SDK
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
   Replace `~/Downloads/zoom-meeting-sdk-linux_x86_64-5.xx.x.xxxx` with your actual extracted folder path.

### 4. Build the Project
Generate build files and compile the demo:
```bash
cd demo
cmake -B build
cd build
make
```
After this, the executable `meetingSDKDemo` will be in `demo/bin/`.

### 5. Generate a JWT Token
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

### 6. Configure the Demo
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

### 7. Run the Demo
Execute the compiled demo:
```bash
cd bin
./meetingSDKDemo
```
If successful, it will authenticate and join the specified meeting. Ensure the meeting is active and you have appropriate permissions.

## Troubleshooting
- **Error: `cannot find -lcurl`**: Install `libcurl4-openssl-dev` (Step 1).
- **Authentication Failure**: Verify your JWT token, meeting number, and that the meeting exists.
- **Raw Data Access**: You need host/co-host rights or a recording token for raw audio/video.

## Notes
- Replace example values (e.g., `1234567890`) with your actual meeting details.
- The JWT token expires (default: 2 hours); regenerate it as needed.
- Refer to the original [README.md](README.md) for additional details.

---

This should give other developers a clear path to set up and run the project. Let me know if you’d like to adjust anything or add more details! Since it’s working for you, do you have a next step in mind, or are we done here?