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
  GIMBALL,//camera view fixed on the center of the object
  FREE,//camera free to move and rotate
};

enum CameraMovement{
  LEFT,
  RIGHT,
  UP,
  DOWN,
};
enum Render_type { main_render_pass, element_detection_pass };
enum Shader_names {
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