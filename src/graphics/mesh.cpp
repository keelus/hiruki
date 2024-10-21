#include "mesh.hpp"
#include "math/vector3.hpp"
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <fstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

namespace Hiruki {
	namespace Graphics {
			std::unique_ptr<Mesh> Mesh::loadFromFile(std::string filename) {
				std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
				mesh->scale = Math::Vector3::one();

				if(!filename.ends_with(".obj")) {
					throw std::invalid_argument("Only OBJ format supported.");
				}

				std::cout << "Parsing model \"" << filename << "\"" << std::endl;

				std::ifstream meshFile(filename);
				std::string line;
				std::vector<TexCoord> texCoords;

				std::unordered_map<std::string, size_t> materialIndexMap;

				// Useful when importing when Blender (right-handed)
				static const bool INVERT_FACES = false;
				static const bool FLIP_X_AXIS = true;

				std::string currentMaterialName = "";
				size_t currentMaterialIndex = 0;

				while(getline(meshFile, line)) {
					if(line.starts_with("v ")) {
						float x, y, z;
						std::sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
						
						if(FLIP_X_AXIS)
							x *= -1;

						mesh->vertices.emplace_back(x, y, z);
					} else if(line.starts_with("vt ")) {
						float x, y;
						std::sscanf(line.c_str(), "vt %f %f", &x, &y);
						texCoords.emplace_back(x, y);
					} else if(line.starts_with("f ")) {
						Math::Vector3i vertexIndices;
						Math::Vector3i textureIndices;
						Math::Vector3i normalIndices;

						std::sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
							&vertexIndices.x, &textureIndices.x, &normalIndices.x,
							&vertexIndices.y, &textureIndices.y, &normalIndices.y,
							&vertexIndices.z, &textureIndices.z, &normalIndices.z
						);
						
						if(INVERT_FACES) {
							std::swap(vertexIndices.y, vertexIndices.z);
							std::swap(textureIndices.y, textureIndices.z);
							std::swap(normalIndices.y, normalIndices.z);
						}

						mesh->faces.emplace_back(
							vertexIndices,
							std::array<TexCoord, 3> {
								texCoords.at(textureIndices.x-1),
								texCoords.at(textureIndices.y-1),
								texCoords.at(textureIndices.z-1)
							},
							currentMaterialIndex
						);
					} else if(line.starts_with("usemtl ")) {
						char materialName[1024];
						std::sscanf(line.c_str(), "usemtl %s", materialName);

						currentMaterialIndex = materialIndexMap.size();
						materialIndexMap[std::string(materialName)] = materialIndexMap.size();
					}
				}

				std::cout << "  - Found: " << std::endl;
				std::cout << "\t- " << mesh->faces.size()<< " faces." << std::endl;
				std::cout << "\t- " << mesh->vertices.size() << " vertices." << std::endl;
				std::cout << "\t- " << materialIndexMap.size() << " materials." << std::endl;

				meshFile.close();
				mesh->parseMaterial(filename, materialIndexMap);
		
				return mesh;
			}

		void Mesh::parseMaterial(std::string objFilename, std::unordered_map<std::string, size_t> materialIndexMap) {
			m_Materials = {};
			std::filesystem::path objPath(objFilename);
			std::filesystem::path basePath = objPath.parent_path();
			std::filesystem::path matFilepath = basePath / (objPath.stem().string() + ".mtl");

			std::cout << "\tParsing material file \"" << matFilepath.string() << "\":" << std::endl;

			std::ifstream matFile(matFilepath);
			std::string line;

			struct MaterialInfo {
				std::string name;
				std::filesystem::path fpath;
			};

			std::optional<MaterialInfo> currentMaterial = std::nullopt;

			while(getline(matFile, line)) {
				if(line.starts_with("newmtl ")) {
					if(currentMaterial.has_value()) {
						if(std::filesystem::path(currentMaterial->fpath).is_absolute()) {
							std::cout << "\t\t Loaded material \"" << currentMaterial->name << "\"." << std::endl;
							m_Materials.emplace(
								std::piecewise_construct,
								std::forward_as_tuple(materialIndexMap.at(currentMaterial->name)),
								std::forward_as_tuple(currentMaterial->name, currentMaterial->fpath)
							);
						} else {
							m_Materials.emplace(
								std::piecewise_construct,
								std::forward_as_tuple(materialIndexMap.at(currentMaterial->name)),
								std::forward_as_tuple(currentMaterial->name, basePath / currentMaterial->fpath)
							);
						}
					}

					char name[256];
					std::sscanf(line.c_str(), "newmtl %s", name);

					currentMaterial = {
						.name = std::string(name),
						.fpath = ".",
					};
				} else if(line.starts_with("map_Kd ")) {
					if(!currentMaterial.has_value())
						throw std::runtime_error("Unexpected material image. Parent material is not defined.");

					char texturePath[2048];
					std::sscanf(line.c_str(), "map_Kd %s", texturePath);

					currentMaterial->fpath =std::string(texturePath); 
				}
			}
			
			// Append also last material
			if(currentMaterial.has_value()) {
				std::cout << "\t\t Loaded material \"" << currentMaterial->name << "\"." << std::endl;
				if(std::filesystem::path(currentMaterial->fpath).is_absolute()) {
					m_Materials.emplace(
						std::piecewise_construct,
						std::forward_as_tuple(materialIndexMap.at(currentMaterial->name)),
						std::forward_as_tuple(currentMaterial->name, currentMaterial->fpath)
					);
				} else {
					m_Materials.emplace(
						std::piecewise_construct,
						std::forward_as_tuple(materialIndexMap.at(currentMaterial->name)),
						std::forward_as_tuple(currentMaterial->name, basePath / currentMaterial->fpath)
					);
				}
			}
		}

		Mesh Mesh::defaultCube() {
			Mesh mesh;
			mesh.scale = Math::Vector3::one();

			mesh.vertices = {
				Math::Vector3(-1, -1, -1),
				Math::Vector3(1, -1, -1),
				Math::Vector3(-1, 1, -1),
				Math::Vector3(1, 1, -1),
				Math::Vector3(-1, -1, 1),
				Math::Vector3(1, -1, 1),
				Math::Vector3(-1, 1, 1),
				Math::Vector3(1, 1, 1),
			};

			mesh.faces = {
				// Front face
				Mesh::Face(Math::Vector3i(3, 1, 4), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}, 0),
				Mesh::Face(Math::Vector3i(1, 2, 4), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}, 0),
				// Right face                                                                       , 0
				Mesh::Face(Math::Vector3i(4, 2, 8), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}, 0),
				Mesh::Face(Math::Vector3i(2, 6, 8), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}, 0),
				// Back face                                                                        , 0
				Mesh::Face(Math::Vector3i(8, 6, 7), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}, 0),
				Mesh::Face(Math::Vector3i(6, 5, 7), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}, 0),
				// Left face                                                                        , 0
				Mesh::Face(Math::Vector3i(7, 5, 3), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}, 0),
				Mesh::Face(Math::Vector3i(5, 1, 3), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}, 0),
				// Top face                                                                         , 0
				Mesh::Face(Math::Vector3i(7, 3, 8), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}, 0),
				Mesh::Face(Math::Vector3i(3, 4, 8), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}, 0),
				// Bottom face                                                                      , 0
				Mesh::Face(Math::Vector3i(1, 5, 2), {TexCoord(0, 1), TexCoord(0, 0), TexCoord(1, 1)}, 0),
				Mesh::Face(Math::Vector3i(5, 6, 2), {TexCoord(0, 0), TexCoord(1, 0), TexCoord(1, 1)}, 0),
			};
			
			return mesh;
		}
	}
}
