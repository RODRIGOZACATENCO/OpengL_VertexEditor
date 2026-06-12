//
// Created by rodrigoz on 6/1/26.
//

#pragma once
enum ElementType {
  FACE,
  VERTEX,
  EDGE,
};
enum GUIState { FACE_EDITING = 1, VERTEX_EDITING = 2, EDGE_EDITING = 3 };
enum CameraMode {
  GIMBALL,//camera view fixed on the center of the object
  FREE,//camera free to move and rotate
};

enum CameraMovement{
  LEFT,
  RIGHT,
  UP,
  DOWN,
};