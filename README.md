VL53L1X ToF depth map viewer for Google Chrome
==============================================

Don't looks at the code, it was a very very very quick draft!

About
-----
Application dispay depth map from 16x16 ToF sensor, 20...25ms per "pixel" (that is 4x4 region, see VL53L1X), so 16*16*(20...25) = 5.12...6.4 seconds per full sensoe read.

![VL53L1X and object](https://sun9-35.userapi.com/k3euErG8EK42cMgq9W8561_0nHX4AmdGZCMKPA/vHlErP7bAlU.jpg)
![depth map](https://sun9-65.userapi.com/B_eYjHAPFyPo5oEhJfY8i_zg0nu3sgK8HHXjdA/_NKOrM-8fVw.jpg)

Notes
-----
Sensor connected to Arduino via TCA9548A, as I want to use more sensors later.
