#include "object.h"

#define PI 3.14159265
using namespace::std;
using namespace::gmtl;


struct _triangle_ {
    gmtl::Vec3f p[3];
    gmtl::Vec3f midpoint;//middle point of triangle
    gmtl::Vec3f normVect; //normal vector of triangle
    float l; //longest side of triangle
    bool rasterize = true;
};


    object::object(string fname) {

       // ifstream myFile(
         //   fname.c_str(), ios::in | ios::binary);

        
        union 
        {
            float _float;
            unsigned int _uint;
        } utof;
        fstream myFile;
        myFile.open(fname, ios::in | ios::binary);

        if (!myFile)
        {
            std::cout << "not found";
        }



        char header_info[80] = "";
        char nTri[4];
        for (int i = 0; i < 4; i++)
        {
            nTri[i] = 1;
        }
        unsigned long nTriLong = 0;

        //read 80 byte header
        myFile.read(header_info, 80);

        //read 4-byte ulong

        myFile.read(nTri, 4);
        nTriLong = *((unsigned long*)nTri);
        _nTri = nTriLong;



     //   _mesh = _tmp;
        
        _mesh = new _triangle_[_nTri];
        _mesh2 = new _triangle_[_nTri];
        _draw_mesh = new _triangle_[_nTri];

        Vec3f max;
        Vec3f min;
        //init min and max

        max = Vec3f(0, 0, 0);
        min = Vec3f(0, 0, 0);


        // read in all the triangles
        //float tmp;
        utof._uint = 0;

        


        for (int i = 0; i < _nTri; i++) {

            char facet[50];

            if (myFile) {

                //read one 50-byte triangle
                myFile.read(facet, 50);
                //read and write triangle points
                int shift = 12;

                for (int j = 0; j < 3; j++)
                {

                    for (int k = 0; k < 3; k++)
                    {
                        //tmp = *((float*)facet + shift);
                        

                        utof._uint |= (uint8_t)facet[shift + 3] << 24;
                        utof._uint |= (uint8_t)facet[shift + 2] << 16;
                        utof._uint |= (uint8_t)facet[shift + 1] << 8;
                        utof._uint |= (uint8_t)facet[shift + 0] << 0;
         

                       // cout << utof._float <<endl;

                        _mesh[i].p[j][k] = utof._float;
                        
                        utof._uint = 0;
                        shift += 4;

                    }
                }
                //read and write triangle normal
                shift = 0;
                for(int j = 0; j < 3; j++)
                {
                    utof._uint |= (uint8_t)facet[shift + 3] << 24;
                    utof._uint |= (uint8_t)facet[shift + 2] << 16;
                    utof._uint |= (uint8_t)facet[shift + 1] << 8;
                    utof._uint |= (uint8_t)facet[shift + 0] << 0;

                    _mesh[i].normVect[j] = utof._float;
                    utof._uint = 0;
                    shift += 4;
                }
                shift = 12;
                
                _mesh[i].midpoint = gmtl::Vec3f(0,0,0);
                //find mesh midpoint (volume) and midpoint of each trianlge
                for (int j = 0; j < 3; j++)
                {
                    //find midpoint of each triangle
                    _mesh[i].midpoint += _mesh[i].p[j];
                    for (int k = 0; k < 3; k++)
                    {
                        //find max and min object points
                        if (_mesh[i].p[j][k] > max[k])
                        {
                            max[k] = _mesh[i].p[j][k];
                        }
                        if (_mesh[i].p[j][k] < min[k])
                        {
                            min[k] = _mesh[i].p[j][k];
                        }
                    }
                }
                _mesh[i].midpoint /= 3;
                int k = 0;
            }
        }
        //find mid point 
        _midPoint = max + min;
        _midPoint /= 2;
    }



    void object::move(float x, float y, float z)
    {
        /*
        _point_ max;
        _point_ min;
        //init min and max

        for (int k = 0; k < 3; k++)
        {
            max.c[k] = 0;
            min.c[k] = 0;
        }
        */

        for (int i = 0; i < _nTri; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                //move triangles
                _mesh[i].p[j][0] += x;
                _mesh[i].p[j][1] += y;
                _mesh[i].p[j][2] += z;

                _mesh[i].midpoint[0] += x;
                _mesh[i].midpoint[1] += y;
                _mesh[i].midpoint[2] += z;

                _mesh[i].normVect[0] += x;
                _mesh[i].normVect[1] += y;
                _mesh[i].normVect[2] += z;
            }
        }

        _midPoint[0] += x;
        _midPoint[1] += y;
        _midPoint[2] += z;
    }

    void object::moveMesh(float x, float y, float z)
    {
        for (int i = 0; i < _nTri; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                //move triangles
                _mesh[i].p[j][0] += x;
                _mesh[i].p[j][1] += y;
                _mesh[i].p[j][2] += z;

                _mesh[i].midpoint[0] += x;
                _mesh[i].midpoint[1] += y;
                _mesh[i].midpoint[2] += z;

                _mesh[i].normVect[0] += x;
                _mesh[i].normVect[1] += y;
                _mesh[i].normVect[2] += z;
            }
        }
    }
    /*
    bool object::alg(object::_triangle_ const& tri1, object::_triangle_ const& tri2)
    {
        return tri1.midpoint[2] >= tri2.midpoint[2];
    }
    */
    void object::rotate(float x, float y, float z)
    {
        std::cout << _nTri;
        //move mesh to center
        moveMesh(-_midPoint[0], -_midPoint[1], -_midPoint[2]);


        //=================rotate===========================

        using namespace std;
        //convert to rad
        float _y = (x * PI) / 180; //y
        float _b = (y * PI) / 180; //b
        float _a = (z * PI) / 180; //a

        //eulers rotation matrix
        float  rot_matrix[3][3];

        rot_matrix[0][0] = cos(_a) * cos(_b);
        rot_matrix[1][0] = sin(_a) * cos(_b);
        rot_matrix[2][0] = -sin(_b);

        rot_matrix[0][1] = cos(_a) * sin(_b) * sin(_y) - sin(_a) * cos(_y);
        rot_matrix[1][1] = sin(_a) * sin(_b) * sin(_y) + cos(_a) * cos(_y);
        rot_matrix[2][1] = cos(_b) * sin(_y);

        rot_matrix[0][2] = cos(_a) * sin(_b) * cos(_y) + sin(_a) * sin(_y);
        rot_matrix[1][2] = sin(_a) * sin(_b) * cos(_y) - cos(_a) * sin(_y);
        rot_matrix[2][2] = cos(_b) * cos(_y);

        //write all zeros to temporary mesh
        for (int i = 0; i < _nTri; i++)
        {

            for (int j = 0; j < 3; j++)
            {
                _mesh2[i].midpoint[j] = 0;
                _mesh2[i].normVect[j] = 0;
                for (int k = 0; k < 3; k++)
                {
                    _mesh2[i].p[j][k] = 0;
                }
            }
        }

        //matrix multiplication

        for (int i = 0; i < _nTri; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; ++k)
                {
                    //calculate and save to tmp mesh
                    for (int l = 0; l < 3; ++l)
                    {
                        //rotate each triangle
                        _mesh2[i].p[j][k] += rot_matrix[k][l] * _mesh[i].p[j][l]; 
                    }
                    //rotate mid point of each triangle
                    _mesh2[i].midpoint[j] += rot_matrix[j][k] * _mesh[i].midpoint[k];
                    //rotate normal vecotrs
                    _mesh2[i].normVect[j] += rot_matrix[j][k] * _mesh[i].normVect[k];
                }
            }
        }


        //save tmp mesh to main mesh
        memcpy(_mesh, _mesh2, sizeof(_triangle_) * _nTri);
        
        //move mesh back

        moveMesh(_midPoint[0], _midPoint[1], _midPoint[2]);
    }


    
    void object::swap(_triangle_* a, _triangle_* b)
    {
        _triangle_ t = *a;
        *a = *b;
        *b = t;
    }

    int object::partition(_triangle_ arr[], int low, int high)
    {
        _triangle_ pivot = arr[high];    // pivot 
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++)
        {
            //if current element is smaller than pivot, increment the low element
            //swap elements at i and j
            //sort by z cord
            if (arr[j].midpoint[2] >= pivot.midpoint[2])
            {
                i++;    // increment index of smaller element 
                swap(&arr[i], &arr[j]);
               
               // NULL;
            }
        }
        swap( &arr[i + 1], &arr[high]);
        return (i + 1);
    }

    //quicksort algorithm
    void object::quickSort(_triangle_ arr[], int start, int end)
    {
        if (start < end)
        {
            //partition the array 
            int pivot = partition(arr, start, end);

            //sort the sub arrays independently 
            quickSort(arr, start, pivot - 1);
            quickSort(arr, pivot + 1, end);
        }
    }

    /*
    Vec2f crossProduct(Vec2f vect_A, Vec2f vect_B)
    {
        Vec2f cross_P;
        cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
        cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
        return cross_P;
    }
    */


    void sortVertice(Vec3f tri[3])
    {
        if (tri[0][1] > tri[1][1])
            swap(tri[0], tri[1]);
        if (tri[0][1] > tri[2][1])
            swap(tri[0], tri[2]);
        if (tri[1][1] > tri[2][1])
            swap(tri[1], tri[2]);
    }


    void fillbotom(SDL_Renderer* _render_, Vec2f tri[3])
    {
        float invslope1 = (tri[1][0] - tri[0][0]) / (tri[1][1] - tri[0][1]);
        float invslope2 = (tri[2][0] - tri[0][0]) / (tri[2][1] - tri[0][1]);

        float curx1 = tri[0][0];
        float curx2 = tri[0][0];

        for (int scanlineY = tri[0][1]+1; scanlineY <= tri[1][1]; scanlineY++)
        {
            SDL_RenderDrawLine(_render_, curx1, scanlineY, curx2, scanlineY);
            curx1 += invslope1;
            curx2 += invslope2;
        }
    }

    void filltop(SDL_Renderer* _render_, Vec2f tri[3])
    {
        float invslope1 = (tri[2][0] - tri[0][0]) / (tri[2][1] - tri[0][1]);
        float invslope2 = (tri[2][0] - tri[1][0]) / (tri[2][1] - tri[1][1]);

        float curx1 = tri[2][0];
        float curx2 = tri[2][0];

        for (int scanlineY = tri[2][1]; scanlineY > tri[0][1]; scanlineY--)
        {
            SDL_RenderDrawLine(_render_, curx1, scanlineY, curx2, scanlineY);
            curx1 -= invslope1;
            curx2 -= invslope2;
        }
    }

    void object::rasterize(SDL_Renderer* _render_, _triangle_ tri)
    {
        /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
        sortVertice(tri.p);

        /* here we know that v1.y <= v2.y <= v3.y */
        /* check for trivial case of bottom-flat triangle */
        Vec2f v1[3];
        v1[0][0] = tri.p[0][0];
        v1[0][1] = tri.p[0][1];
        v1[1][0] = tri.p[1][0];
        v1[1][1] = tri.p[1][1];
        v1[2][0] = tri.p[2][0];
        v1[2][1] = tri.p[2][1];
        Vec2f v2[3];

        if (v1[1][1] == v1[2][1])
        {
            fillbotom(_render_,v1);
        }
        /* check for trivial case of top-flat triangle */
        else if (v1[0][1] == v1[1][1])
        {
            filltop(_render_, v1);
        }
        else
        {
            /* general case - split the triangle in a topflat and bottom-flat one */
            v2[0] = v1[0];
            v2[1] = v1[1];

            Vec2f v3;


            v3 = Vec2f(
                (int)(tri.p[0][0] + ((float)(tri.p[1][1] - tri.p[0][1]) / (float)(tri.p[2][1] - tri.p[0][1])) * (tri.p[2][0] - tri.p[0][0])), tri.p[1][1]);

            v2[2] = v3;
            fillbotom(_render_, v2);
            v2[0] = v1[1];
            v2[1] = v3;
            v2[2] = v1[2];

            filltop(_render_, v2);
        }
    }


    float lenght2d(Vec3f vec)
    {
        //c^2 = a^2+b^2
        return sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
    }


    float rend_div_coef = 1000;
    float rend_sum_coef = 0.01;
    /*
    void object::sort(object::_triangle_ mesh[], uint32_t n)
    {
        std::sort(mesh, mesh + sizeof(object::_triangle_) * n);
    }
    */

  
    float tangent(gmtl::Vec3f v1, gmtl::Vec3f v2)
    {
        gmtl::normalize(v1);
        gmtl::normalize(v2);
        return gmtl::length(gmtl::Vec3f(v2 - v1));
    }

    void object::draw(SDL_Renderer* _render_) {

        //foreach triangle
        for (int i = 0; i < _nTri; i++)
        {
            float a = 0;
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    //projection 
                    _draw_mesh[i].p[j][k] = -_mesh[i].p[j][k] / ((_mesh[i].p[j][2] / rend_div_coef) + rend_sum_coef);
                    
                    _draw_mesh[i].normVect = _mesh[i].normVect;

                    _draw_mesh[i].midpoint[k] = -_mesh[i].midpoint[k] / ((_mesh[i].midpoint[2] / rend_div_coef) + rend_sum_coef);
                    _draw_mesh[i].midpoint[2] = _mesh[i].midpoint[2];

                }
            }
        }


        gmtl::Vec3f _draw_light;
        _draw_light[0] = -light[0] / ((light[2] / rend_div_coef) + rend_sum_coef);
        _draw_light[1] = -light[1] / ((light[2] / rend_div_coef) + rend_sum_coef);
 
       quickSort(_draw_mesh, 0, _nTri);
       // sort(_draw_mesh, _nTri);
        
        //scale image to screen

        float Kx = 1;
        float Ky = 1;
        float px = 500;
        float py = 250;

        for (int i = 0; i < _nTri; i++)
        {
            for (int j = 0; j < 3; j++)
            {

                _draw_mesh[i].p[j][0] = (Kx * _draw_mesh[i].p[j][0]) + px;
                _draw_mesh[i].p[j][1] = (Ky * _draw_mesh[i].p[j][1]) + py;
            }
        }

        //sort triangles to get proper overlay
       // std::sort(_draw_mesh, _draw_mesh + sizeof(_triangle_) * _nTri, &object::triangle_sort);




        //draw wireframe to screen
        //SDL_SetRenderDrawColor(_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);
        float lightning;
        for (int i = 0; i < _nTri; i++)
        {
            lightning = tangent(_mesh[i].normVect, light - _mesh[i].midpoint) - 2;

            lightning *= -255;
            // lightning = 1 / lightning;
            if (i == _nTri)
            {
                SDL_SetRenderDrawColor(_render_, lightning, 0, 0, SDL_ALPHA_OPAQUE);
            }
            else {
                SDL_SetRenderDrawColor(_render_, lightning, lightning, lightning, SDL_ALPHA_OPAQUE);
            }
            rasterize(_render_, _draw_mesh[i]);
        }
        SDL_SetRenderDrawColor(_render_, 255, 0, 0, SDL_ALPHA_OPAQUE);

        
        SDL_RenderDrawPoint(_render_, _draw_light[0], _draw_light[1]);
      
          /*
        for (int j = 0; j < 3; j++)
        {
            //divide summ of all point to count
            tmp[j] /= 3;
        }
        */
        //tmp /= 3;
      //  float t = 0.0003;
        std::cout << " t: " << tangent(_mesh[1].normVect, _mesh[1].midpoint - light);;
        std::cout << " x: " << _draw_light[_nTri-2];
        std::cout << " y: " << _draw_light[_nTri-2];
        std::cout << " z: " << _draw_light[_nTri-2] << std::endl;


        SDL_SetRenderDrawColor(_render_, 255, 0, 0, SDL_ALPHA_OPAQUE);



        //midpoint
       // cout << "|||" << _midPoint[0] << "|" << _midPoint[1] << "|" << _midPoint[2] << endl;
    }






