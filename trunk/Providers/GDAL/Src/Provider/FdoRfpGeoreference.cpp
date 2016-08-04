#include "FDORFP.h"
#include "FdoRfpGeoreference.h"

FdoRfpGeoreference::FdoRfpGeoreference(void): m_hasCoord(false),
	m_resX(0.0), m_resY(0.0), m_insX(0.0), m_insY(0.0),
	m_rotX(0.0), m_rotY(0.0), m_toleranceXY(0.001), m_toleranceZ(0.001)
{
}

FdoRfpGeoreference::~FdoRfpGeoreference(void)
{
}
