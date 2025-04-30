#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <cmath>
#include <algorithm>

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;


// Verifica lunghezza spigoli > 0
bool EdgesValid(const PolygonalMesh& mesh)
{		
	for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i)
	{
		const unsigned int startIdx = mesh.Cell1DsExtrema(0, i);
		const unsigned int endIdx = mesh.Cell1DsExtrema(1, i);
		
		
		if (startIdx >= mesh.Cell0DsCoordinates.cols() || endIdx >= mesh.Cell0DsCoordinates.cols())
		{
			cerr << "Indice non valido tra gli estremi dell'arco: " 
			<< "inizio = " << startIdx << ", fine = " << endIdx
			<< ", limite massimo = " << mesh.Cell0DsCoordinates.cols() - 1  << endl;
			return false;
		}
		
		
		Vector2d point1 = mesh.Cell0DsCoordinates.block<2, 1>(0, startIdx);
		Vector2d point2 = mesh.Cell0DsCoordinates.block<2, 1>(0, endIdx);
			
		double edgeLenght = (point2 - point1).norm();
			
		if (edgeLenght < 1e-16)
		{
			cerr << "Spigolo con lunghezza nulla tra i punti " << startIdx << " e " << endIdx << endl;
			return false;
		}
	}
	return true;
}
    

// Verifica area poligono > 0
bool PolygonsValid(const PolygonalMesh& mesh)
{

    for (const auto& [id, vertexIds] : mesh.Cell2DsVertices)
    {        
		// Un poligono deve avere almeno 3 vertici
        if (vertexIds.size() < 3)
        {
            cerr << "Poligono con meno di 3 vertici all'indice " << id << endl;
            return false;
        }

        double total = 0.0;
		const size_t count = vertexIds.size();

        for (size_t idx = 0; idx < count; ++idx)
        {
            const unsigned int current = vertexIds[idx];
            const unsigned int next = vertexIds[(idx + 1) % count];
			
			Vector2d pointA = mesh.Cell0DsCoordinates.block<2, 1>(0, current);
			Vector2d pointB = mesh.Cell0DsCoordinates.block<2, 1>(0, next);

			
            total += pointA.x() * pointB.y() - pointB.x() * pointA.y();

        }
		
        double area = 0.5 * fabs(total);

        if (area < 1e-16)
        {
            cerr << "Poligono con area nulla all'indice " << id << endl;
            return false;
        }
    }
    return true;
}

// Verifica Marker
bool MarkersValid(const PolygonalMesh& mesh)
{
    cout << "Marker registrati:" << endl;
    for (const auto& [marker, listIds] : mesh.MarkerCell0Ds)
    {
        cout << "Marker0D: " << marker << " IDs = [";
        for (auto& id: listIds)
            cout << ' ' << id;
        cout << " ]" << endl;
    };
                      
    cout << endl;
                      
    for (const auto& [marker, listIds] : mesh.MarkerCell1Ds)
    {
        cout << "Marker1D: " << marker << " IDs = [";
        for (auto& id: listIds)
            cout << ' ' << id;
        cout << " ]" << endl;
    }
    cout << endl;
    return true;
                      
}


int main()
{
    PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        cerr << "File non trovato" << endl;
        return false;
    }
	 
	//Verifica validità spigoli
	if (!EdgesValid(mesh))
	{
		cerr << "Errore: spigoli di lunghezza nulla " << endl;
		return false;
	}else {
		cout << "Ogni spigolo ha lunghezza diversa da zero" << endl;
	}
	
	
	//Verifica validità area
	if (!PolygonsValid(mesh))
	{
		cerr << "Errore: poligoni di area nulla " << endl;
		return false;
	}else {
		cout << "Ogni poligono ha area diversa da zero" << endl;
	}
	
	
	// Verifica validità marker
    if (!MarkersValid(mesh))
	{
		cerr << "Errore: marker non validi." << endl;
		return false;
		}else {
			cout << "Tutti i marker sono correttamente memorizzati" << endl;
		}
	 
	
	Gedim::UCDUtilities utilities;
	
	utilities.ExportPoints("./Cell0Ds.inp", 
							mesh.Cell0DsCoordinates);
	
	utilities.ExportSegments("./Cell1Ds.inp", 
							mesh.Cell0DsCoordinates, 
							mesh.Cell1DsExtrema);
    
	return true;
}
	
