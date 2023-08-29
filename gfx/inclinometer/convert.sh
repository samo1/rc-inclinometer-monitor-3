#!/usr/bin/env bash

set -Eeuo pipefail

ffmpeg -i inclinometer_jeep_pitch.png -pix_fmt rgb565 inclinometer_jeep_pitch_rgb565.bmp
tail -c $((100*100*2)) < inclinometer_jeep_pitch_rgb565.bmp > inclinometer_jeep_pitch_rgb565.raw

ffmpeg -i inclinometer_jeep_roll.png -pix_fmt rgb565 inclinometer_jeep_roll_rgb565.bmp
tail -c $((100*100*2)) < inclinometer_jeep_roll_rgb565.bmp > inclinometer_jeep_roll_rgb565.raw
