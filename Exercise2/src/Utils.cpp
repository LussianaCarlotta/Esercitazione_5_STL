#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary
{
bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

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

    list<vector<string>> listLines;

    string line;
	getline(file, line);
	
    while (getline(file, line))
	{
		stringstream ss(line);
		string tmp;
		vector<string> val;
		while(getline (ss, tmp, ';'))
		{
			val.push_back(tmp);
		}
        listLines.push_back(val);
	}
	
    file.close();
	
	
    mesh.NumCell0Ds = listLines.size(); 

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no Cell 0D" << endl;
        return false;
    }
	
	// dimensiono:
    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (const vector<string>& line : listLines)
    {
        istringstream converter_id(line[0]);
		istringstream converter_marker(line[1]);
		istringstream converter_coordX(line[2]);
		istringstream converter_coordY(line[3]);
		
		unsigned int id;
		unsigned int marker;
		Vector2d coord;
		
		converter_id >> id;
		converter_marker >> marker;
		converter_coordX >> mesh.Cell0DsCoordinates(0, id);
		converter_coordY >> mesh.Cell0DsCoordinates(1, id);
		
		mesh.Cell0DsId.push_back(id);
		

    /// Memorizza i marker
    if (marker != 0) 
    {
        // devo inserire il nuovo id nella lista corrispondente alla chiave marker
        auto it = mesh.MarkerCell0Ds.find(marker);
        if(it != mesh.MarkerCell0Ds.end())
        {
            (*it).second.push_back(id);
        }
        else 
		{
            mesh.MarkerCell0Ds.insert({marker, {id}});
        }
    }
    }

    return true; 
}
// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell1Ds.csv");

    if(file.fail())
        return false;

    list<vector<string>> listLines;
    string line;
	
	getline(file, line);
    while (getline(file, line))
	{
		stringstream ss(line);
		string tmp;
		vector<string> val;
		while(getline (ss, tmp, ';'))
		{
			val.push_back(tmp);
		}
        listLines.push_back(val);
	}	
    file.close();


    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (const vector<string>& line : listLines)
    {
		istringstream converter_id(line[0]);
		istringstream converter_marker(line[1]);
		istringstream converter_origin(line[2]);
		istringstream converter_end(line[3]);
		
        unsigned int id;
		unsigned int marker;
		Vector2d vertices;
		
		converter_id >> id;
		converter_marker >> marker;
		converter_origin >> mesh.Cell1DsExtrema(0, id);
		converter_end >> mesh.Cell1DsExtrema(1, id);
		
		mesh.Cell1DsId.push_back(id);

		
    /// Memorizza i marker
	if (marker != 0) 
	{
		auto it = mesh.MarkerCell1Ds.find(marker);
        if(it != mesh.MarkerCell1Ds.end())
        {
            (*it).second.push_back(id);
        }
        else {
            mesh.MarkerCell1Ds.insert({marker, {id}});
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

    list<vector<string>> listLines;
    string line;
	getline(file, line);
	
    while (getline(file, line))
	{
		stringstream ss(line);
		string tmp;
		vector<string> val;
		while(getline (ss, tmp, ';'))
		{
			val.push_back(tmp);
		}
        listLines.push_back(val);
	}
    file.close();
	

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const vector<string>& line : listLines)
    {
		istringstream converter_id(line[0]);
		istringstream converter_marker(line[1]);
		
		unsigned int id;
		unsigned int marker;
		
		converter_id >> id;
		converter_marker >> marker;
		
 
		istringstream converter_numVertices(line[2]);
		unsigned int numVertices;
		converter_numVertices >> numVertices;
		
		istringstream converter_numEdges(line[4]);
		unsigned int numEdges;
		converter_numEdges >> numEdges;
		
		istringstream converter_vertices(line[3]);
		istringstream converter_edges(line[5]);
		
		array<unsigned int, 8> vertices;
		array<unsigned int, 8> edges;
		
		for (unsigned int i = 3; i < 8; i++)
		{
			converter_vertices >> vertices[i];
		}
		
		for (unsigned int i = 3; i < 8; i++)
		{
			converter_edges >> edges[i];
		}
		
        mesh.Cell2DsId.push_back(id);
		mesh.Cell2DsNumVertices.push_back(numVertices);
        mesh.Cell2DsVertices.push_back(vertices);
		mesh.Cell2DsNumEdges.push_back(numEdges);
        mesh.Cell2DsEdges.push_back(edges);
	
	/// Memorizza i marker
	if (marker != 0) 
	{
		auto it = mesh.MarkerCell2Ds.find(marker);
        if(it != mesh.MarkerCell2Ds.end())
        {
            (*it).second.push_back(id);
        }
        else {
            mesh.MarkerCell2Ds.insert({marker, {id}});
        }
    }
	}
	
    return true;
}

}