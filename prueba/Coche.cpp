#include "Coche.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

const float ESCALA = 1000.0f / 32.0f;   
const float ELEVACION = 0.3f;          


PiezaCoche::PiezaCoche(std::vector<float>& v, std::vector<float>& n, std::vector<float>& t, std::vector<unsigned short>& idx)
{
	numVertices = (GLuint)(v.size() / 3);
	numFaces = (GLuint)(idx.size() / 3);
	vertices = new GLfloat[v.size()];
	normals = new GLfloat[n.size()];
	textures = new GLfloat[t.size()];
	indexes = new GLushort[idx.size()];
	for (int i = 0; i < (int)v.size(); i++) vertices[i] = v[i];
	for (int i = 0; i < (int)n.size(); i++) normals[i] = n[i];
	for (int i = 0; i < (int)t.size(); i++) textures[i] = t[i];
	for (int i = 0; i < (int)idx.size(); i++) indexes[i] = idx[i];
	InitBuffers();
}

Coche::Coche()
{
}


Coche::~Coche()
{
	for (int i = 0; i < (int)piezas.size(); i++)
		delete piezas[i];
	for (int i = 0; i < (int)materiales.size(); i++)
	{
		GLuint textura = materiales[i]->GetTexture();
		glDeleteTextures(1, &textura);
		delete materiales[i];
	}
}

int Coche::GetNumPieces()
{
	return (int)piezas.size();
}

CGPiece* Coche::GetPiece(int i)
{
	return piezas[i];
}


bool Coche::Cargar(std::string carpeta)
{
	std::string linea;

	
	std::map<std::string, std::string> texturaDe;
	std::ifstream ficheroMtl((carpeta + "Car.mtl").c_str());
	if (!ficheroMtl)
	{
		std::cout << "No se encuentra el fichero " << carpeta << "Car.mtl" << std::endl;
		return false;
	}
	std::string material = "";
	while (std::getline(ficheroMtl, linea))
	{
		std::istringstream ss(linea);
		std::string palabra;
		ss >> palabra;
		if (palabra == "newmtl")
		{
			ss >> material;
		}
		else if (palabra == "map_Kd")
		{
			std::string nombre;
			if (ss >> nombre)
				texturaDe[material] = nombre;
		}
	}

	
	std::ifstream ficheroObj((carpeta + "Car.obj").c_str());
	if (!ficheroObj)
	{
		std::cout << "No se encuentra el fichero " << carpeta << "Car.obj" << std::endl;
		return false;
	}
	std::vector<std::string> lineas;
	while (std::getline(ficheroObj, linea))
		lineas.push_back(linea);

	
	float minY = 1e9f, maxY = -1e9f, minZ = 1e9f;
	for (int i = 0; i < (int)lineas.size(); i++)
	{
		std::istringstream ss(lineas[i]);
		std::string palabra;
		ss >> palabra;
		if (palabra == "v")
		{
			float x, y, z;
			ss >> x >> y >> z;
			if (y < minY) minY = y;
			if (y > maxY) maxY = y;
			if (z < minZ) minZ = z;
		}
	}
	float centroY = (minY + maxY) / 2.0f;

	
	std::vector<glm::vec3> posiciones;
	std::vector<glm::vec3> normales;
	std::vector<glm::vec2> uvs;


	struct DatosPieza {
		std::vector<float> v, n, t;
		std::vector<unsigned short> idx;
		std::map<long long, int> indiceDe;
	};
	std::map<std::string, DatosPieza> datos;
	std::string actual = "";

	for (int i = 0; i < (int)lineas.size(); i++)
	{
		std::istringstream ss(lineas[i]);
		std::string palabra;
		ss >> palabra;

		if (palabra == "v")
		{
			float x, y, z;
			ss >> x >> y >> z;
			posiciones.push_back(glm::vec3(-(y - centroY) * ESCALA,
				(z - minZ) * ESCALA + ELEVACION,
				-x * ESCALA));
		}
		else if (palabra == "vn")
		{
			float x, y, z;
			ss >> x >> y >> z;
			normales.push_back(glm::vec3(-y, z, -x));
		}
		else if (palabra == "vt")
		{
			float u, v;
			ss >> u >> v;
			uvs.push_back(glm::vec2(u, v));
		}
		else if (palabra == "usemtl")
		{
			ss >> actual;
		}
		else if (palabra == "f")
		{
			
			std::vector<int> indicesCara;
			std::string token;
			DatosPieza& pieza = datos[actual];
			while (ss >> token)
			{
				
				for (int k = 0; k < (int)token.size(); k++)
				{
					if (token[k] == '/')
						token[k] = ' ';
				}
				std::istringstream indices(token);
				int a = 0, b = 0, c = 0;
				indices >> a >> b >> c;

				long long clave = ((long long)a * 100000 + b) * 100000 + c;
				std::map<long long, int>::iterator it = pieza.indiceDe.find(clave);
				if (it == pieza.indiceDe.end())
				{
					int nuevo = (int)(pieza.v.size() / 3);
					pieza.indiceDe[clave] = nuevo;
					pieza.v.push_back(posiciones[a - 1].x);
					pieza.v.push_back(posiciones[a - 1].y);
					pieza.v.push_back(posiciones[a - 1].z);
					pieza.n.push_back(normales[c - 1].x);
					pieza.n.push_back(normales[c - 1].y);
					pieza.n.push_back(normales[c - 1].z);
					pieza.t.push_back(uvs[b - 1].x);
					pieza.t.push_back(uvs[b - 1].y);
					indicesCara.push_back(nuevo);
				}
				else
				{
					indicesCara.push_back(it->second);
				}
			}
			
			for (int k = 1; k + 1 < (int)indicesCara.size(); k++)
			{
				pieza.idx.push_back((unsigned short)indicesCara[0]);
				pieza.idx.push_back((unsigned short)indicesCara[k]);
				pieza.idx.push_back((unsigned short)indicesCara[k + 1]);
			}
		}
	}

	
	std::map<std::string, DatosPieza>::iterator it;
	for (it = datos.begin(); it != datos.end(); it++)
	{
		std::string nombre = it->first;
		if (nombre == "glow")
			continue;   

		if (it->second.v.size() / 3 > 65535)
		{
			std::cout << "La pieza " << nombre << " tiene demasiados vértices" << std::endl;
			return false;
		}

		CGMaterial* mat = new CGMaterial();
		mat->SetAmbientReflect(1.0f, 1.0f, 1.0f);
		mat->SetDifusseReflect(1.0f, 1.0f, 1.0f);
		mat->SetSpecularReflect(0.3f, 0.3f, 0.3f);
		mat->SetShininess(16.0f);
		if (nombre == "glass")
			mat->SetDissolved(0.9f);   
		if (texturaDe.count(nombre) > 0)
			mat->InitTexture((carpeta + texturaDe[nombre]).c_str());
		materiales.push_back(mat);

		CGPiece* pieza = new PiezaCoche(it->second.v, it->second.n, it->second.t, it->second.idx);
		pieza->SetMaterial(mat);
		piezas.push_back(pieza);
	}

	std::cout << "Coche cargado desde " << carpeta << ": " << piezas.size() << " piezas" << std::endl;
	return true;
}
