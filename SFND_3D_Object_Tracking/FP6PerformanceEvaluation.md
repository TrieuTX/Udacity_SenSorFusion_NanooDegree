In Project SFND_2D_Feature_Tracking:
## **Top Three Detector/Descriptor Combinations:**
Detector/Descriptor  | Number of Matched Keypoints | Processing Time |
-------------------- | --------------------------- | --------------- |
**FAST+BRIEF**           | 1099 keypoints              | 15.6447 ms      |
**FAST+BRISK**           | 899 keypoints               | 24.1113 ms      |
**FAST+ORB**             | 1081 keypoints              | 41.0496 ms      |

# Performance Evaluation Top Three Top Three Detector/Descriptor Combinations.
## Detector = FAST, Descriptor = BRISK
Frame | Image | Number Lidar points | TTC Lidar | TTC Camera
:---:  | :---:  | :---: | ---:  | ---: 
1 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_1.png ) | 340 | 12.289100 | 12.300000
2 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_2.png ) | 309 | 13.354725 | 12.345282
3 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_3.png ) | 326 | 16.384452 | 16.616273
4 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_4.png ) | 322 | 14.076445 | 12.885663
5 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_5.png ) | 344 | 12.729945 | -inf
6 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_6.png ) | 352 | 13.751074 | 13.038573
7 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_7.png ) | 317 | 13.731432 | 12.040983
8 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_8.png ) | 312 | 13.790126 | 11.406610
9 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_9.png ) | 311 | 12.058994 | 11.868356
10 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_10.png ) | 286 | 11.864185 | 13.347326
11 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_11.png ) | 302 | 11.968197 | 12.949216
12 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_12.png ) | 302 | 9.887113 | 12.117402
13 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_13.png ) | 316 | 9.425037 | 12.778433
14 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_14.png ) | 312 | 9.302148 | 11.607725
15 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_15.png ) | 289 | 8.321200 | 11.407860
16 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_16.png ) | 297 | 8.898673 | 12.256560
17 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_17.png ) | 279 | 11.030114 | 9.293304
18 |![](PerformanceEvaluation/FAST+BRISK/FAST+BRISK+FRAME_18.png ) | 303 | 8.535568 | 11.860611


## Detector = FAST, Descriptor = BRIEF
Frame | Image | Number Lidar points | TTC Lidar | TTC Camera
:---:  | :---:  | :---: | ---:  | ---: 
1 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_1.png ) | 340 | 12.289100 | 11.177575
2 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_2.png ) | 309 | 13.354725 | 13.006897
3 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_3.png ) | 326 | 16.384452 | 14.820583
4 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_4.png ) | 322 | 14.076445 | 13.248256
5 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_5.png ) | 344 | 12.729945 | -inf
6 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_6.png ) | 352 | 13.751074 | 41.782307
7 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_7.png ) | 317 | 13.731432 | 12.758325
8 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_8.png ) | 312 | 13.790126 | 12.766404
9 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_9.png ) | 311 | 12.058994 | 13.923081
10 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_10.png ) | 286 | 11.864185 | 16.214137
11 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_11.png ) | 302 | 11.968197 | 13.635546
12 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_12.png ) | 302 | 9.887113 | 12.983142
13 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_13.png ) | 316 | 9.425037 | 13.148670
14 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_14.png ) | 312 | 9.302148 | 11.703404
15 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_15.png ) | 289 | 8.321200 | 12.607115
16 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_16.png ) | 297 | 8.898673 | 14.551571
17 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_17.png ) | 279 | 11.030114 | 11.341063
18 |![](PerformanceEvaluation/FAST+BRIEF/FAST+BRIEF+FRAME_18.png ) | 303 | 8.535568 | 13.790759

## Detector = FAST, Descriptor = ORB
Frame | Image | Number Lidar points | TTC Lidar | TTC Camera
:---:  | :---:  | :---: | ---:  | ---: 
1 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_1.png ) | 340 | 12.289100 | 11.999320
2 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_2.png ) | 309 | 13.354725 | 12.979305
3 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_3.png ) | 326 | 16.384452 | 26.572394
4 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_4.png ) | 322 | 14.076445 | 14.244157
5 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_5.png ) | 344 | 12.729945 | -inf
6 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_6.png ) | 352 | 13.751074 | 55.978844
7 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_7.png ) | 317 | 13.731432 | 12.426376
8 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_8.png ) | 312 | 13.790126 | 12.186040
9 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_9.png ) | 311 | 12.058994 | 12.871126
10 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_10.png ) | 286 | 11.864185 | -inf
11 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_11.png ) | 302 | 11.968197 | -inf
12 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_12.png ) | 302 | 9.887113 | 13.163589
13 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_13.png ) | 316 | 9.425037 | 13.562908
14 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_14.png ) | 312 | 9.302148 | 11.111480
15 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_15.png ) | 289 | 8.321200 | 10.843857
16 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_16.png ) | 297 | 8.898673 | 11.393874
17 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_17.png ) | 279 | 11.030114 | 12.165587
18 |![](PerformanceEvaluation/FAST+ORB/FAST+ORB+FRAME_18.png ) | 303 | 8.535568 | 109.968649

