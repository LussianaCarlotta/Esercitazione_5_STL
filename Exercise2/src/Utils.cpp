#include "PolygonalMesh.hpp"
#include "Utils.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <Eigen/Dense>

namespace PolygonalLibrary
{
//Importa le mesh
bool ImportMesh(PolygonalMesh& mesh)
{
	// Importa i vertici 
    if(!ImportCell0Ds(mesh))
        return false;

	// Importa gli spigoli
    if(!ImportCell1Ds(mesh))
        return false;
	
	// Importa le celle
    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}
// ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell0Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
	
	// Legge tutte le righe del file
    while (getline(file, line))
	{
        listLines.push_back(line);
	}
	
    file.close();
	
	// Rimuove l'header
	listLines.pop_front();
	
    mesh.NumCell0Ds = listLines.size(); 

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no Cell 0D" << endl;
        return false;
    }
	
    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (string& line : listLines)
    {
		replace(line.begin(), line.end(), ';', ' ');
		istringstream converter(line);
		
		unsigned int id;
		unsigned int marker;
		Vector2d coord;
		
		converter >> id >> marker >> mesh.Cell0DsCoordinates(0, id) >> mesh.Cell0DsCoordinates(1, id);
		
		mesh.Cell0DsId.push_back(id);
		

    // Memorizza i marker in una mappa
		if (marker != 0) 
		{
			auto it = mesh.MarkerCell0Ds.find(marker);
			if(it == mesh.MarkerCell0Ds.end())
			{
				mesh.MarkerCell0Ds.insert({marker, {id}});
			}
			else 
			{
				it->second.push_back(id);
			}
		}
    }

    return true; 
}
// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell1Ds.csv");

    if(file.fail()){
        return false;
	}
	
	list<string> listLines;
    string line;
	
	// Legge tutte le righe del file
    while (getline(file, line))
	{
        listLines.push_back(line);
	}
    file.close();
	
	// Rimuove l'header
	listLines.pop_front();


    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (string& line : listLines)
    {
		replace(line.begin(), line.end(), ';', ' ');
        istringstream converter(line);
		
        unsigned int id;
		unsigned int marker;
		Vector2d vertices;
		
		converter >> id >> marker >> mesh.Cell1DsExtrema(0, id) >> mesh.Cell1DsExtrema(1, id);
		
		mesh.Cell1DsId.push_back(id);

		
    // Memorizza i marker in una mappa
		if (marker != 0) 
		{
			auto it = mesh.MarkerCell1Ds.find(marker);
			if(it == mesh.MarkerCell1Ds.end())
			{
				mesh.MarkerCell1Ds.insert({marker, {id}});
			}
			else {
				it->second.push_back(id);
			}
		}
    }

    return true;
}
// ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell2Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
	
	// Legge tutte le righe del file
    while (getline(file, line))
	{
        listLines.push_back(line);
	}
    file.close();
	
	// Rimuove l'header
	listLines.pop_front();
	

    mesh.NumCell2Ds = listLines.size();

    if (listLines.empty())
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }
	
	// Pulisce
	mesh.Cell2DsVertices.clear();
    mesh.Cell2DsEdges.clear();

    for (string& line : listLines)
    {
		replace(line.begin(), line.end(), ';', ' ');
        istringstream converter(line);
		
		unsigned int id;
		unsigned int marker;
		unsigned int numVertices;
		unsigned int numEdges;
		
		converter >> id >> marker >> numVertices;
		
		vector<unsigned int> vertices(numVertices);
		for (unsigned int i = 0; i < numVertices; i++)
		{
			converter >> vertices[i];
		}
		
		converter >> numEdges;
		
		vector<unsigned int> edges(numEdges);
		for (unsigned int i = 0; i < numEdges; i++)
		{
			converter >> edges[i];
		}
		
        mesh.Cell2DsVertices[id] = vertices;
        mesh.Cell2DsEdges[id] = edges;
	}

	
    return true;
}

}