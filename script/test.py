#!/usr/bin/env python
# -*- coding: utf-8 -*-
import pyrealsense2 as rs
import numpy as np
import cv2

# ストリーム(IR/Color/Depth)の設定
config = rs.config()

config.enable_stream(rs.stream.infrared, 1, 640, 480, rs.format.y8, 30)
config.enable_stream(rs.stream.infrared, 2, 640, 480, rs.format.y8, 30)
config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)

# ストリーミング開始
pipeline = rs.pipeline()
profile = pipeline.start(config)


try:
    while True:
        # フレーム待ち
        frames = pipeline.wait_for_frames()
        #IR１
        ir_frame1 = frames.get_infrared_frame(1)
        ir_image1 = np.asanyarray(ir_frame1.get_data())

        #IR2
        ir_frame2 = frames.get_infrared_frame(2)
        ir_image2 = np.asanyarray(ir_frame2.get_data())

        #RGB
        RGB_frame = frames.get_infrared_frame()
        RGB_image = np.asanyarray(ir_frame2.get_data())

        #Depth
        depth_frame = frames.get_depth_frame()
        depth_image = np.asanyarray(depth_frame.get_data())

        #depth imageをカラーマップに変換
        depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.08), cv2.COLORMAP_JET)

        # 表示
        cv2.namedWindow('RealSense', cv2.WINDOW_AUTOSIZE)
        cv2.imshow('RealSense', depth_colormap)

        if cv2.waitKey(1) & 0xff == 27:
            cv2.destroyAllWindows()
            break

finally:
    # ストリーミング停止
    pipeline.stop()
