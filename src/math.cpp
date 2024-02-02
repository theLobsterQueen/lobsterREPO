// INCLUDES SELF DEFINITION //
#include <math.h>

// FUNCTION IMPLEMENTATIONS //
LobMatrix math :: identityMatrix()
{
	// CREATES A FOUR-DIMENSIONAL IDENTITY MATRIX //
	return LobMatrix
	{
		std :: vector<float>
		{
			// FIRST COLUMN //
			1, 0, 0, 0,

			// SECOND COLUMN //
			0, 1, 0, 0,

			// THIRD COLUMN //
			0, 0, 1, 0,

			// FOURTH COLUMN //
			0, 0, 0, 1
		},
		4, 4
	};
}

LobMatrix math :: inverse(LobMatrix inputMat)
{
	LobMatrix tempMat = inputMat;
	for(char x = 0; x < 4; x++)
		for(char y = 0; y < 4; y++)
			tempMat.setPoint(y, x, inputMat.getPoint(x, y));

	return tempMat;
}

float math :: dot(LobMatrix matY, LobMatrix matX, unsigned int yIndex, unsigned int xIndex)
{
	// ASSERTS VALID INPUT //
	if(xIndex > matX.getCols() || yIndex > matY.getRows())
	{
		std :: cout << "ERROR! ATTEMPTED TO USE AN INVALID INDEX FOR DOT MULTIPLICATION!"
			<< std :: endl;
		return 0;
	}

	if(matX.getCols() != matY.getRows())
	{
		std :: cout << "ERROR! ATTEMPTED DOT CALCULATION BETWEEN TWO VECTORS "
			<< "BUT VECTORS ARE NOT OF EQUIVALENT SIZE!" << std :: endl;
		return 0;
	}

	// VARIABLE INITIALIZATION //
	float totalValue = 0;

	// BEGINS DOT CALCULATION //
	for(int i = 0; i < matX.getCols(); i++)
		totalValue += matY.getPoint(yIndex, i) * matX.getPoint(i, xIndex);

	// RETURNS TOTAL DOT VALUE //
	return totalValue;
}

LobMatrix math :: rotateMatrix(std :: vector<float> axis, float angle)
{
	// VARIABLE INITIALIZATION //
	LobMatrix tempMat = identityMatrix(); 

	// READS DATA FOR AXIS ROTATION //
	float ax = abs(axis[0]);
	float ay = abs(axis[1]);
	float az = abs(axis[2]);

	// CREATES PERPENDICULAR VECTOR //
	std :: vector<float> rotPer = { 0, 0, 0 };
	if(ax <= ay && ax <= az)
		rotPer = std :: vector<float> { 0, -axis[2], axis[1] };
	
	else if(ay <= ax && ay <= az)
		rotPer = std :: vector<float> { -axis[2], 0, axis[0] };

	else
		rotPer = std :: vector<float> { -axis[1], axis[0], 0 };	

	// CREATES CROSS VECTOR //
	std :: vector<float> cVec = math :: cross(axis, rotPer);
	
	// CREATES ALIGNMENT ROTATION MATRIX //
	LobMatrix aRotMat =
	{
		std :: vector<float>
		{
			// FIRST COLUMN //
			cVec[0], cVec[1], cVec[2], 0.0f,

			// SECOND COLUMN //
			rotPer[0], rotPer[1], rotPer[2], 0.0f,

			// THIRD COLUMN //
			axis[0], axis[1], axis[2], 0.0f,

			// FOURTH COLUMN //
			0, 0, 0, 1
		},
		4, 4
	};

	// CREATES ROTATION MATRIX ABOUT Z-AXIS //
	LobMatrix zRotMat =
	{
		std :: vector<float>
		{
			// FIRST COLUMN //
			(float) (cos(angle)), (float) (-(sin(angle))), 0.0f, 0.0f,

			// SECOND COLUMN //
			(float) (sin(angle)), (float) (cos(angle)), 0.0f, 0.0f,

			// THIRD COLUMN //
			0, 0, 1, 0.0f,

			// FOURTH COLUMN //
			0, 0, 0, 1
		},
		4, 4
	};

	// RETURNS THE ROTATED MATRIX //
	return (aRotMat * zRotMat * math :: inverse(aRotMat));
}

std :: vector<float> math :: cross(std :: vector<float> x, std :: vector<float> y)
{
	// VARIABLE INITIALIZATION //
	std :: vector<float> newVec = { 0, 0, 0 };

	// ASSERTS VALIDITY OF INPUT //
	if(x.size() != 3 || y.size() != 3)
	{
		std :: cout << "ERROR! AT LEAST ONE OF THE INPUT VECTORS IS NOT EQUAL TO "
			<< "THREE!" << std :: endl;
		return std :: vector<float>();
	}

	// CREATES THE CROSS VECTOR //
	newVec[0] = (x[1] * y[2]) - (x[2] * y[1]);	
	newVec[1] = (x[2] * y[0]) - (x[0] * y[2]);
	newVec[2] = (x[0] * y[1]) - (x[1] * y[0]);

	// RETURNS IT //
	return newVec;
}

float math :: toRadians(float x)
{
	x *= 180;
	x /= 3.14;
	return x;
}
