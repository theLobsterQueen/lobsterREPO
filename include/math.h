#ifndef LOBSTER_MATH_H
#define LOBSTER_MATH_H 1

/// <summary> ///
///		This file contains a collections of structs and systems to facilitate
///			mathematical operations in the engine backend. 
///
///		NOTE: I manually wrote all this stuff so that I could understand it better.
///			I despise black-boxes, especially in something so critical to an engine's
///			operation like 3D math.
/// </summary> ///

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <iostream> 
#include <vector>
#include <stdlib.h>
#include <cmath>

// CLASS PROTOTYPES //
class LobMatrix;

// FUNCTION DEFINITIONS //
namespace math
{
	float dot(LobMatrix matY, LobMatrix matX, unsigned int yIndex, unsigned int xIndex);
	float dot(std :: vector<float> x, std :: vector<float> y);
	std :: vector<float> scaleVec(std :: vector<float> x, float s);
	std :: vector<float> cross(std :: vector<float> x, std :: vector<float> y);
	std :: vector<float> normalize(std :: vector<float> x);
	std :: vector<float> vecByMat(std :: vector<float> lhs, LobMatrix rhs);
	LobMatrix identityMatrix();
	LobMatrix inverse(LobMatrix inputMat);
	LobMatrix rotateMatrix(std :: vector<float> axis, float inputAngle);
	float toRadians(float x);
};

// STRUCT DEFINITIONS //

/// <summary> ///
///		Matrix, organized in column-major order to better translate to GLSL.
/// </summary> ///

class LobMatrix
{
	public :
		// CONSTRUCTOR/DESTRUCTOR ARGUMENTS //
		LobMatrix

		// PARAMETERS //
		(
			std :: vector<float> inputData, 
			unsigned int inputNumCols, unsigned int inputNumRows
		)

		{
			// NO-THOUGHT VARIABLE ASSIGNMENTS //
			numCols = inputNumCols;
			numRows = inputNumRows;

			// ASSERTS VALIDITY OF INPUT //
			if(inputData.size() != (numCols * numRows))
			{
				std :: cout << "ERROR! ATTEMPTED TO CREATE MATRIX DATA WITH VECTOR THAT "
					"WAS NOT THE SAME SIZE AS THE DIMENSIONS OF THE MATRIX!" 
					<< std :: endl;
				return;
			}

			// IF VALID, ASSIGNS DATA //
			data = inputData;
		}

		// CONSTRUCTOR WITH NO PRESET DATA //
		LobMatrix(unsigned int inputNumCols, unsigned int inputNumRows)
		{
			// NO-THOUGHT VARIABLE ASSIGNMENTS //
			numCols = inputNumCols;
			numRows = inputNumRows;

			unsigned int size = numCols * numRows;
			for(int i = 0; i < size; i++)
				data.push_back(0);
		}

		// PUBLIC METHODS //
		void out()
		{
			// PRINTS ENTRY GUARD //
			std :: cout << "/";
			for(int i = 0; i < numRows * 2; i++)
				std :: cout << "-"; 
			std :: cout << "\\" << std :: endl;

			for(int y = 0; y < numRows; y++)
			{
				std :: cout << " ";
				for(int x = 0; x < numCols; x++)
					std :: cout << getPoint(y, x) << " ";
				std :: cout << std :: endl;
			}

			std :: cout << "\\";
			for(int i = 0; i < numRows * 2; i++)
				std :: cout << "-"; 
			std :: cout << "/" << std :: endl;
		}

		// GET/SET METHODS //
		std :: vector<float> getVec()
			{ return data; }

		float * getData()
			{ return data.data(); }

		unsigned int getCols()
			{ return numCols; }

		unsigned int getRows()
			{ return numRows; }

		std :: vector<float> getRow(unsigned int rowIndex)
		{
			std :: vector<float> rowVec;
			for(int i = 0; i < numCols; i++)
				rowVec.push_back(data[(numCols * rowIndex) + i]);

			return rowVec;
		}

		std :: vector<float> getCol(unsigned int colIndex)
		{
			std :: vector<float> colVec;
			for(int i = 0; i < numRows; i++)
				colVec.push_back(data[(numRows * colIndex) + i]);

			return colVec;
		}

		float getPoint(unsigned int y, unsigned int x)
			{ return data[(numRows * x) + y]; }

		void setPoint(unsigned int y, unsigned int x, float value)
			{ data[(numRows * x) + y] = value; }

		// OPERATOR OVERLOADS //
		LobMatrix operator *(LobMatrix rhs)
		{
			// VARIABLE INITIALIZATION //
			LobMatrix newMat = LobMatrix((*this).numCols, rhs.numRows);

			// ASSERTS VALID INPUT //
			if((*this).numCols != rhs.numRows)
			{
				std :: cout << "ERROR: ATTEMPTED MATRIX MULTIPLICATION, BUT NUMBER OF "
					<< "COLUMNS ON LEFT MATRIX DOES NOT EQUAL NUMBER OF ROWS ON "
					<< "RIGHT MATRIX!" << std :: endl;
				return newMat;
			}

			for(int x = 0; x < newMat.numCols; x++)
				for(int y = 0; y < newMat.numRows; y++)
					newMat.setPoint(y, x, math :: dot((*this), rhs, y, x));

			// RETURNS MULTIPLIED VALUE //
			return newMat;
		}

		LobMatrix& operator *=(LobMatrix rhs)
		{
			(*this) = (*this) * rhs;
			return (*this);
		}

	private :
		// UTILITY METHODS //


		// PRIVATE ATTRIBUTES //
		std :: vector<float> data;
		unsigned int numRows = 0;
		unsigned int numCols = 0;
};

#endif
