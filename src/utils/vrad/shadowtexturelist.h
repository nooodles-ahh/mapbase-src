#pragma once
#include "mathlib/vector.h"
#include "studio.h"
#include "tier1/utldict.h"
#include <mathlib/vmatrix.h>

// keeps a list of all textures that cast shadows via alpha channel
class CShadowTextureList
{
public:
	// This loads a vtf and converts it to RGB8888 format
	unsigned char *LoadVTFRGB8888( const char *pName, int *pWidth, int *pHeight, bool *pClampU, bool *pClampV );

	// Checks the database for the material and loads if necessary
	// returns true if found and pIndex will be the index, -1 if no alpha shadows
	bool FindOrLoadIfValid( const char *pMaterialName, int *pIndex, int *pIndex2 = nullptr );

	// iterate the textures for the model and load each one into the database
	// this is used on models marked to cast texture shadows
	void LoadAllTexturesForModel( studiohdr_t *pHdr, int *pTextureList );
	int AddMaterialEntry( int shadowTextureIndex, const Vector2D &t0, const Vector2D &t1, const Vector2D &t2 );

	// HACKHACK: Compute the average coverage for this triangle by sampling the AABB of its texture space
	float ComputeCoverageForTriangle( int shadowTextureIndex, const Vector2D &t0, const Vector2D &t1, const Vector2D &t2 );
	int SampleMaterial( int materialIndex, const Vector &coords, bool bBackface );

	struct alphatexture_t
	{
		short width;
		short height;
		bool allowBackface;
		bool clampU;
		bool clampV;
		unsigned char *pAlphaTexels;
		VMatrix transform;

		void InitFromRGB8888( int w, int h, unsigned char *pTexels )
		{
			width = w;
			height = h;
			pAlphaTexels = new unsigned char[w * h];
			for ( int i = 0; i < h; i++ )
			{
				for ( int j = 0; j < w; j++ )
				{
					int index = ( i * w ) + j;
					pAlphaTexels[index] = pTexels[index * 4 + 3];
				}
			}
			transform.Identity();
		}
	};
	struct materialentry_t
	{
		int textureIndex;
		Vector2D uv[3];
	};
	// this is the list of textures we've loaded
	// only load each one once
	CUtlDict< alphatexture_t, unsigned short > m_Textures;
	CUtlVector<materialentry_t> m_MaterialEntries;
};

extern CShadowTextureList g_ShadowTextureList;