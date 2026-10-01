//
// Created by rodrigoz on 6/1/26.
//

#pragma once
enum ElementType {
  FACE,
  VERTEX,
  EDGE,
};

enum FrameBuffers {
  MAIN_COLOR_BUFFER,
  ELEMENT_DETECTION_BUFFER

};
enum GUIState { FACE_EDITING = 1, VERTEX_EDITING = 2, EDGE_EDITING = 3 };

enum CameraMode {
  NONE,
  GIMBALL,//camera view fixed on the center of the object
  FREE,//camera free to move and rotate
};

enum KeyNames{
  LEFT,
  RIGHT,
  UP,
  DOWN,
  TAB,
};
enum KeyState {
  IDLE,
  PRESSED,
  HOLD,
  RELEASED

};
enum RenderType { main_render_pass, element_detection_pass };
enum ShaderNames {
  face_color_pass,
  edge_color_pass,
  vertex_color_pass,
  edge_detection,
  vertex_detection,
  face_detection,
  axis_lines_shader,
  render_window,
  shader_count
};