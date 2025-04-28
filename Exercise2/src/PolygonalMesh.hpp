#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary {

struct PolygonalMesh
{
	unsigned int NumCell0Ds = 0;
	vector<unsigned int> Cell0DsId = {};
	MatrixXd Cell0DsCoordinates = {};
	map<unsigned int, list<unsigned int>> MarkerCell0Ds = {};
	
	unsigned int NumCell1Ds = 0;
	vector<unsigned int> Cell1DsId = {};
	MatrixXi Cell1DsExtrema = {};
	map<unsigned int, list<unsigned int>> MarkerCell1Ds = {};
	
	unsigned int NumCell2Ds = 0;
	vector<unsigned int> Cell2DsId = {};
	vector<unsigned int> Cell2DsNumVertices = {};
	vector<unsigned int> Cell2DsNumEdges = {};
	vector<array<unsigned int, 8 >> Cell2DsVertices = {};
	vector<array<unsigned int, 8 >> Cell2DsEdges = {};
	map<unsigned int, list<unsigned int>> MarkerCell2Ds = {};
 
};

}
