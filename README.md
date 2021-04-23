# DeepFake ![build](https://img.shields.io/github/workflow/status/florianvazelle/deep-fake/build?label=Windows&logo=github)

Real-time face swapping application between a video and an image, built with OpenCV and dlib.

## Building

### Build and run the standalone target

Use the following command to build and run the executable target.

```bash
cmake -Bbuild
cmake --build build
./build/bin/deep_fake -h
```
