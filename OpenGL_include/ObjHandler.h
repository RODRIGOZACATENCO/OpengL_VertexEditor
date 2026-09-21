#pragma once
#include <memory>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <variant>

enum num_type {
    INT,
    FLOAT
};


struct RawMeshData {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> vertex_normals;
    std::vector<int> faces;

    friend std::ostream& operator<<(std::ostream& os, const RawMeshData& raw_mesh_data) { return os; }
};

//reads the obj file and returns 3 vectors represeting the object to contruct
class ObjHandler {
    RawMeshData data;
    std::string file_str;
    unsigned int curr_pos = 0;
    const std::string meshes_path = "../meshes/";

public:
    void readFile(const std::string& file_path) {
        std::ifstream file(meshes_path + file_path);
        if (!file) { throw std::runtime_error("Could not open file"); }
        std::stringstream buffer;
        buffer << file.rdbuf();
        file_str = buffer.str();
    }

    glm::vec3 parseVec3() {
        glm::vec3 v;
        do { curr_pos++; }
        while (file_str[curr_pos] == ' ');
        v.x = parseNumber();
        do { curr_pos++; }
        while (file_str[curr_pos] == ' ');
        v.y = parseNumber();
        do { curr_pos++; }
        while (file_str[curr_pos] == ' ');
        v.z = parseNumber();
        return v;
    }

    float parseNumber(num_type num_type = FLOAT) {
        std::string str;
        str += file_str[curr_pos];
        curr_pos++;
        bool dot_flag = false;
        while (curr_pos < file_str.size() && (std::isdigit(file_str[curr_pos]) || file_str[curr_pos] == '.' && !
            dot_flag)) {
            if (file_str[curr_pos] == '.') dot_flag = true;
            str += file_str[curr_pos];
            curr_pos++;
        }
        if (num_type == FLOAT) return std::stof(str);
        else return std::stoi(str);
    }


    RawMeshData getObjectInfo(const std::string& file_name) {
        curr_pos = 0;
        readFile(file_name);
        unsigned int file_size = file_str.size();
        try {
            while (curr_pos < file_size) {
                char curr_char = file_str[curr_pos];
                if (curr_char == 'v') {
                    if (file_str[curr_pos + 1] == 'n') {
                        //vertex normals
                        curr_pos += 2;
                        data.vertex_normals.push_back(parseVec3());
                    }
                    else {
                        //vertex
                        curr_pos++;
                        data.vertices.push_back(parseVec3());
                    }
                }
                if (curr_char == 'f') {
                    //faces
                    curr_pos++;
                    do { curr_pos++; }
                    while (file_str[curr_pos] == ' ');
                    data.faces.push_back(parseNumber(INT) - 1);
                    curr_pos += 3;
                    do { curr_pos++; }
                    while (file_str[curr_pos] == ' ');
                    data.faces.push_back(parseNumber(INT) - 1);
                    curr_pos += 3;
                    do { curr_pos++; }
                    while (file_str[curr_pos] == ' ');
                    data.faces.push_back(parseNumber(INT) - 1);
                    curr_pos += 3;
                }
                if (curr_char == '\n') { curr_pos++; }
                else { curr_pos++; }
            }
        }
        catch (std::exception& e) { std::cout << e.what(); }
        return {data};
    }
};
