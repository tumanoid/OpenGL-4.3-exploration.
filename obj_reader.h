#ifndef OBJ_READER_H
#define OBJ_READER_H

#include <stdio.h>

typedef unsigned long  DWORD;

class objNode
{
public:
    objNode(){}
    ~objNode(){ delete[] vtx; delete[] face;}
    void ObjRead(const char* fName);
public:
    DWORD             countVertex;
    DWORD             countFace;

    float*            vtx; // vX,vY,vZ, nX,nY,nZ, u,v, bright
    DWORD*            face;
};


void objNode::ObjRead(const char* fName)
{
    countVertex=0;
    countFace=0;
    DWORD countNormals=0;
    DWORD countUV=0;

    char buffer[256];
	FILE *fp;

    fopen_s(&fp,fName,"r");
    
    if(!fp)
    {
        printf("ERROR: file %s not found\n", fName);
        return;
    }

    while(!feof(fp))
	{
		// read next line
		fgets(buffer,256,fp);
        if (feof(fp)) break;

		//	Normals. Look for the 'vn '
		if( strncmp("vn ",buffer,3) == 0 )
		{
			countNormals++;
		}

        // UVs. Look for the 'uv '
		if( strncmp("vt ",buffer,3) == 0 )
		{
			countUV++;
		}

        // Vertex. Look for the 'v '
		if( strncmp("v ",buffer,2) == 0 )
		{
			countVertex++;
		}
		
        // Faces. Look for the 'f '
		if( strncmp("f ",buffer,2) == 0 )
		{
			countFace++;
		}
	}// while
  
    printf("V:%i N:%i UV:%i FACES:%i\n", countVertex, countNormals, countUV, countFace);

	float* vertex = new float[countVertex*3]; memset(vertex,0,sizeof(float)*countVertex*3);
    float* uv     = new float[countUV*2]; memset(uv,0,sizeof(float)*countUV*2);
    float* normal = new float[countNormals*3]; memset(normal,0,sizeof(float)*countNormals*3);
    DWORD* ff     = new DWORD[countFace*(3+3+2)]; memset(ff,0,sizeof(DWORD)*countFace*(3+3+2));

    
    //fflush(fp);
    //fclose(fp);
    //fopen_s(&fp,fName,"r");
    fseek(fp,0,SEEK_SET);
    

    float a,b,c;
	int vc(0), nc(0), tc(0), fc(0);
    DWORD v1(0),uv1(0),n1(0);
    DWORD v2(0),uv2(0),n2(0);
    DWORD v3(0),uv3(0),n3(0);

    //while(!feof(fp))
    
    while(1)
	{
        fgets(buffer,256,fp);     
        
        if (feof(fp)) break;
        
        if( strncmp("v ",buffer,2) == 0 )
		{
			
            sscanf((buffer+1),"%f%f%f",&a,&b,&c);
	
            vertex[ vc * 3 + 0 ]=a;
			vertex[ vc * 3 + 1 ]=b;
    		vertex[ vc * 3 + 2 ]=c;
			++vc;
            
		}
        
        if( strncmp("vt ",buffer,3) == 0 )
		{
	
            sscanf((buffer+2),"%f%f",&a,&b);
	
            uv[ tc * 2 + 0 ]=a;
			uv[ tc * 2 + 1 ]=b;
			++tc;
            
		}

        if( strncmp("vn ",buffer,3) == 0 )
		{
	
            sscanf((buffer+2),"%f%f%f",&a,&b,&c);
	
            normal[ nc * 3 + 0 ]=a;
			normal[ nc * 3 + 1 ]=b;
    		normal[ nc * 3 + 2 ]=c;
			++nc;
            
		}

		
        if( strncmp("f ",buffer,2) == 0 )
		{
            sscanf((buffer+1),"%d/%d/%d %d/%d/%d %d/%d/%d",&v1,&uv1,&n1, &v2,&uv2,&n2, &v3,&uv3,&n3);
	
            ff[ fc * 9 + 0 ]=v1-1;
			ff[ fc * 9 + 1 ]=v2-1;
    		ff[ fc * 9 + 2 ]=v3-1;

            ff[ fc * 9 + 3 ]=uv1-1;
			ff[ fc * 9 + 4 ]=uv2-1;
    		ff[ fc * 9 + 5 ]=uv3-1;

            ff[ fc * 9 + 6 ]=n1-1;
			ff[ fc * 9 + 7 ]=n2-1;
    		ff[ fc * 9 + 8 ]=n3-1;
			
            fc++;            
		}
    }
    
    //fflush(fp);   
    fclose(fp);

    printf("data read OK\n");
            
        FILE* fff=fopen("BAAAA.mdl","w");
        for (int i=0;i<countVertex; i++)
            fprintf(fff,"v %f %f %f\n", vertex[i*3+0], vertex[i*3+1], vertex[i*3+2]);
        
        for (int i=0;i<countNormals; i++)
            fprintf(fff,"n %f %f %f\n", normal[i*3+0], normal[i*3+1], normal[i*3+2]);
        
        for (int i=0;i<countUV; i++)
            fprintf(fff,"uv %f %f \n", uv[i*2+0], uv[i*2+1]);

        for (int i=0;i<countFace; i++){
            fprintf(fff,"V face N%i %u %u %u \n", i, ff[i*9+0], ff[i*9+1], ff[i*9+2]);
            fprintf(fff,"N face N%i %u %u %u \n", i, ff[i*9+6], ff[i*9+7], ff[i*9+8]);
        }
        fclose(fff);     
    

    if ((vc>=tc) && (vc>=nc))
    {
        printf("!!!!!Vertex Count >>>>\n");

        countVertex=countVertex;

        vtx  = new float[countVertex*(3+3+2+1)];// vertex/normal/uv/bright     
        face = new DWORD[countFace*3];

        for (int i=0; i<countFace; ++i)
        {
            
            v1=ff[ i * 9 + 0 ];
			v2=ff[ i * 9 + 1 ];
    		v3=ff[ i * 9 + 2 ];

            uv1=ff[ i * 9 + 3 ];
			uv2=ff[ i * 9 + 4 ];
    		uv3=ff[ i * 9 + 5 ];

            n1=ff[ i * 9 + 6 ];
			n2=ff[ i * 9 + 7 ];
    		n3=ff[ i * 9 + 8 ];
                        
            // push data
            // first vertex
            face[i*3+0]=v1;
                vtx[v1*9+0]=vertex[v1*3+0];//x
                vtx[v1*9+1]=vertex[v1*3+1];//y
                vtx[v1*9+2]=vertex[v1*3+2];//x
                
                vtx[v1*9+3]=normal[n1*3+0];//x
                vtx[v1*9+4]=normal[n1*3+1];//y
                vtx[v1*9+5]=normal[n1*3+2];//z
                
                vtx[v1*9+6]=uv[uv1*2+0];//u
                vtx[v1*9+7]=uv[uv1*2+1];//v
                
                vtx[v1*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 
            
            // second vertex
            face[i*3+1]=v2;
                vtx[v2*9+0]=vertex[v2*3+0];
                vtx[v2*9+1]=vertex[v2*3+1];
                vtx[v2*9+2]=vertex[v2*3+2];
                
                vtx[v2*9+3]=normal[n2*3+0];
                vtx[v2*9+4]=normal[n2*3+1];
                vtx[v2*9+5]=normal[n2*3+2];
                
                vtx[v2*9+6]=uv[uv2*2+0];
                vtx[v2*9+7]=uv[uv2*2+1];
                
                vtx[v2*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 

            // third vertex
            face[i*3+2]=v3;
                vtx[v3*9+0]=vertex[v3*3+0];
                vtx[v3*9+1]=vertex[v3*3+1];
                vtx[v3*9+2]=vertex[v3*3+2];
                
                vtx[v3*9+3]=normal[n3*3+0];
                vtx[v3*9+4]=normal[n3*3+1];
                vtx[v3*9+5]=normal[n3*3+2];
                
                vtx[v3*9+6]=uv[uv3*2+0];
                vtx[v3*9+7]=uv[uv3*2+1];
                
                vtx[v3*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 
        }//for
    }//if
    

    if ((nc>=vc) && (nc>=tc))
    {
        
        printf("!!!!!Normal Count >>>>\n");
        countVertex=countNormals;
        
        printf("%i %i\n",countFace,countVertex);

        face = new DWORD[countFace*3];
        vtx  = new float[countVertex*(3+3+2+1)];// vertex/normal/uv/bright               
        
        printf("!!!!!Normal Count >>>>\n");

        for (int i=0; i<countFace; ++i)
        {
            
            v1=ff[ i * 9 + 0 ]; if (v1>=countVertex) printf("V1");
			v2=ff[ i * 9 + 1 ]; if (v2>=countVertex) printf("V2");
    		v3=ff[ i * 9 + 2 ]; if (v2>=countVertex) printf("V3");

            uv1=ff[ i * 9 + 3 ]; if (uv1>=countUV) printf("UV1");
            uv2=ff[ i * 9 + 4 ]; if (uv2>=countUV) printf("UV2");
    		uv3=ff[ i * 9 + 5 ]; if (uv3>=countUV) printf("UV3");

            n1=ff[ i * 9 + 6 ]; if (n1>=countNormals) printf("N1 %i", n1);
			n2=ff[ i * 9 + 7 ]; if (n2>=countNormals) printf("N2 %i", n2);
    		n3=ff[ i * 9 + 8 ]; if (n3>=countNormals) printf("N3 %i", n3);
                        
            // push data
            // first vertex
            face[i*3+0]=n1;
                vtx[n1*9+0]=vertex[v1*3+0];//x
                vtx[n1*9+1]=vertex[v1*3+1];//y
                vtx[n1*9+2]=vertex[v1*3+2];//x
                
                vtx[n1*9+3]=normal[n1*3+0];//x
                vtx[n1*9+4]=normal[n1*3+1];//y
                vtx[n1*9+5]=normal[n1*3+2];//z
                
                vtx[n1*9+6]=uv[uv1*2+0];//u
                vtx[n1*9+7]=uv[uv1*2+1];//v
                
                vtx[n1*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 
            
            // second vertex
            face[i*3+1]=n2;
                vtx[n2*9+0]=vertex[v2*3+0];
                vtx[n2*9+1]=vertex[v2*3+1];
                vtx[n2*9+2]=vertex[v2*3+2];
                
                vtx[n2*9+3]=normal[n2*3+0];
                vtx[n2*9+4]=normal[n2*3+1];
                vtx[n2*9+5]=normal[n2*3+2];
                
                vtx[n2*9+6]=uv[uv2*2+0];
                vtx[n2*9+7]=uv[uv2*2+1];
                
                vtx[n2*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 

            // third vertex
            face[i*3+2]=n3;
                vtx[n3*9+0]=vertex[v3*3+0];
                vtx[n3*9+1]=vertex[v3*3+1];
                vtx[n3*9+2]=vertex[v3*3+2];
                
                vtx[n3*9+3]=normal[n3*3+0];
                vtx[n3*9+4]=normal[n3*3+1];
                vtx[n3*9+5]=normal[n3*3+2];
                
                vtx[n3*9+6]=uv[uv3*2+0];
                vtx[n3*9+7]=uv[uv3*2+1];
                
                vtx[n3*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 
        }//for
    }//if
    
    printf("!!!!!OK>>>>\n");


    if ((tc>=nc) && (tc>=vc))
    {      
        printf("!!!!!UV Count >>>>\n");
        countVertex=countUV;

        vtx = new float[countVertex*(3+3+2+1)];// vertex/normal/uv/bright     
            memset(vtx,0,sizeof(float)*countVertex*(3+3+2+1));
        
        face = new DWORD[countFace*3];
            memset(face,0,sizeof(DWORD)*countFace*3);

        for (int i=0; i<countFace; ++i)
        {
           
            v1=ff[ i * 9 + 0 ]; if (v1>=countVertex) printf("V1");
			v2=ff[ i * 9 + 1 ]; if (v2>=countVertex) printf("V2");
    		v3=ff[ i * 9 + 2 ]; if (v2>=countVertex) printf("V3");

            uv1=ff[ i * 9 + 3 ]; if (uv1>=countUV) printf("UV1");
            uv2=ff[ i * 9 + 4 ]; if (uv2>=countUV) printf("UV2");
    		uv3=ff[ i * 9 + 5 ]; if (uv3>=countUV) printf("UV3");

            n1=ff[ i * 9 + 6 ]; if (n1>=countNormals) printf("N1 %i", n1);
			n2=ff[ i * 9 + 7 ]; if (n2>=countNormals) printf("N2 %i", n2);
    		n3=ff[ i * 9 + 8 ]; if (n3>=countNormals) printf("N3 %i", n3);
                        
            // push data
            
            // first vertex
            face[i*3+0]=uv1;
                vtx[uv1*9+0]=vertex[v1*3+0];//x
                vtx[uv1*9+1]=vertex[v1*3+1];//y
                vtx[uv1*9+2]=vertex[v1*3+2];//x
                
                vtx[uv1*9+3]=normal[n1*3+0];//x
                vtx[uv1*9+4]=normal[n1*3+1];//y
                vtx[uv1*9+5]=normal[n1*3+2];//z
                
                vtx[uv1*9+6]=uv[uv1*2+0];//u
                vtx[uv1*9+7]=uv[uv1*2+1];//v
                
                vtx[uv1*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 
            
            // second vertex
            face[i*3+1]=uv2;
                vtx[uv2*9+0]=vertex[v2*3+0];
                vtx[uv2*9+1]=vertex[v2*3+1];
                vtx[uv2*9+2]=vertex[v2*3+2];
                
                vtx[uv2*9+3]=normal[n2*3+0];
                vtx[uv2*9+4]=normal[n2*3+1];
                vtx[uv2*9+5]=normal[n2*3+2];
                
                vtx[uv2*9+6]=uv[uv2*2+0];
                vtx[uv2*9+7]=uv[uv2*2+1];
                
                vtx[uv2*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 

            // third vertex
            face[i*3+2]=uv3;
                vtx[uv3*9+0]=vertex[v3*3+0];
                vtx[uv3*9+1]=vertex[v3*3+1];
                vtx[uv3*9+2]=vertex[v3*3+2];
                
                vtx[uv3*9+3]=normal[n3*3+0];
                vtx[uv3*9+4]=normal[n3*3+1];
                vtx[uv3*9+5]=normal[n3*3+2];
                
                vtx[uv3*9+6]=uv[uv3*2+0];
                vtx[uv3*9+7]=uv[uv3*2+1];
                
                vtx[uv3*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 
                
        }//for
    }//if

  
        
        for (int i=0;i<countFace; i++)
        {
            printf("face %i-----------------------------\n",i);
            printf("vertA %f %f %f\n", vtx[face[i*3+0]*9+0], vtx[face[i*3+0]*9+1], vtx[face[i*3+0]*9+2]);
            //printf("vertB %f %f %f\n", vtx[face[i*3+1]*9+0], vtx[face[i*3+1]*9+1], vtx[face[i*3+1]*9+2]);
            //printf("vertC %f %f %f\n", vtx[face[i*3+2]*9+0], vtx[face[i*3+2]*9+1], vtx[face[i*3+2]*9+2]);

            //printf("norm %f %f %f\n", vtx[i*3+0],vtx[i*3+1],vtx[i*3+2]);
            //printf("uv   %f %f\n", vtx[i*6+0],vtx[i*6+1]);
        }

    delete [] vertex;
    delete [] uv;
    delete [] normal;
    delete [] ff;
    

    printf("Read model is OK.\nTotal vertex is %i\n",countVertex);
    printf("Total face is %i\n",countFace);
         

    return;

}

#endif//OBJ_READER_H

/*
if ((vc>=tc) && (vc>=nc))
    {
        
        printf("!!!!!Vertex Count >>>>\n");

        countVertex=countVertex;

        vtx = new float[countVertex*(3+3+2+1)];// vertex/normal/uv/bright     
        face   = new DWORD[countFace*3];

        for (int i=0; i<countFace; ++i)
        {
 			unsigned int v1,uv1,n1;
            unsigned int v2,uv2,n2;
            unsigned int v3,uv3,n3;
            
            v1=ff[ i * 9 + 0 ];
			v2=ff[ i * 9 + 1 ];
    		v3=ff[ i * 9 + 2 ];

            uv1=ff[ i * 9 + 3 ];
			uv2=ff[ i * 9 + 4 ];
    		uv3=ff[ i * 9 + 5 ];

            n1=ff[ i * 9 + 6 ];
			n2=ff[ i * 9 + 7 ];
    		n3=ff[ i * 9 + 8 ];
                        
            // push data
            
            // first vertex
            face[i*3+0]=v1;
                vtx[v1*9+0]=vertex[v1*3+0];//x
                vtx[v1*9+1]=vertex[v1*3+1];//y
                vtx[v1*9+2]=vertex[v1*3+2];//x
                
                vtx[v1*9+3]=normal[n1*3+0];//x
                vtx[v1*9+4]=normal[n1*3+1];//y
                vtx[v1*9+5]=normal[n1*3+2];//z
                
                vtx[v1*9+6]=uv[uv1*2+0];//u
                vtx[v1*9+7]=uv[uv1*2+1];//v
                
                vtx[v1*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 
            
            // second vertex
            face[i*3+1]=v2;
                vtx[v2*9+0]=vertex[v2*3+0];
                vtx[v2*9+1]=vertex[v2*3+1];
                vtx[v2*9+2]=vertex[v2*3+2];
                
                vtx[v2*9+3]=normal[n2*3+0];
                vtx[v2*9+4]=normal[n2*3+1];
                vtx[v2*9+5]=normal[n2*3+2];
                
                vtx[v2*9+6]=uv[uv2*2+0];
                vtx[v2*9+7]=uv[uv2*2+1];
                
                vtx[v2*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 

            // third vertex
            face[i*3+2]=v3;
                vtx[v3*9+0]=vertex[v3*3+0];
                vtx[v3*9+1]=vertex[v3*3+1];
                vtx[v3*9+2]=vertex[v3*3+2];
                
                vtx[v3*9+3]=normal[n3*3+0];
                vtx[v3*9+4]=normal[n3*3+1];
                vtx[v3*9+5]=normal[n3*3+2];
                
                vtx[v3*9+6]=uv[uv3*2+0];
                vtx[v3*9+7]=uv[uv3*2+1];
                
                vtx[v3*9+8]=float( (rand()+0.001)/(RAND_MAX+0.001) );// rand[0-1] 
        }//for
    }//if
*/