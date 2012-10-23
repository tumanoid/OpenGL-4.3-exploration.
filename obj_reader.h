#ifndef OBJ_READER_H
#define OBJ_READER_H

#include <stdio.h>
#include <string.h>

typedef unsigned long  DWORD;

typedef struct obj_s 
{
    DWORD             cVertex;
    DWORD             cFace;
    //float*            vtx; // vX,vY,vZ, nX,nY,nZ, u,v
    float*            vtx; // vX,vY,vZ, bright
    DWORD*            face;
} obj_t;



obj_t& ObjRead(const char* fName)
{
	int vc=0, nc=0, tc=0, fc=0;
    
    int countVertex=0;
    int countFace=0;
    int countNormals=0;
    int countUV=0;

    obj_t tmpObj;
    memset(&tmpObj,0, sizeof(tmpObj));
	
    char buffer[256];
	FILE *fp = NULL;

    fp = fopen(fName,"r");
    
    if(!fp)
    {
        printf("ERROR: file %s not found\n", fName);
        return tmpObj;
    }

    while(!feof(fp))
	{
		// read next line
		fgets(buffer,256,fp);

		//	Normals. Look for the 'vn '
		if( strncmp("vn ",buffer,3) == 0 )
		{
			countNormals++;
		}
		else

        // UVs. Look for the 'uv '
		if( strncmp("vt ",buffer,3) == 0 )
		{
			countUV++;
		}
		else

        // Vertex. Look for the 'v '
		if( strncmp("v ",buffer,2) == 0 )
		{
			countVertex++;
		}
		else
		
        // Faces. Look for the 'f '
		if( strncmp("f ",buffer,2) == 0 )
		{
			countFace++;
		}
	}// while
    
    fclose(fp);
       
    fp = fopen(fName,"r");

	float* vv = new float[countVertex*4];

    while(!feof(fp))
	{
		fgets(buffer,256,fp);     
        if( strncmp("v ",buffer,2) == 0 )
		{
			float a,b,c;
            sscanf((buffer+1),"%f%f%f",&a,&b,&c);
	
            vv[ vc * 4 + 0 ]=a;
			vv[ vc * 4 + 1 ]=b;
    		vv[ vc * 4 + 2 ]=c;
            vv[ vc * 4 + 3 ]=(rand() % 100)/99.9999;// rand[0-1]

			++vc;
		}

    }

    fclose(fp);
    
    printf("OK:\n");

    tmpObj.cVertex = countVertex;
    tmpObj.vtx = vv;
    
    return tmpObj;
}

#endif//OBJ_READER_H