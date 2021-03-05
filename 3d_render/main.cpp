#include "main.h"
#include "object.h"


SDL_Window* window;
SDL_Renderer* render;

#define PI 3.14159265


int cycle();



//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

SDL_Joystick* gGameController = NULL;

object obj("untitled.stl");


int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK)) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    window = SDL_CreateWindow("CUM", 100, 100, 1000, 500, SDL_WINDOW_SHOWN);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return 1;
    }

    if (render == nullptr) {
        SDL_DestroyWindow(window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(render);
        SDL_Quit();
        return 1;
    }


    if (SDL_NumJoysticks() < 1)
    {
        std::cout << "Warning: No joysticks connected!\n";
    }
    else
    {
        //Load joystick
        gGameController = SDL_JoystickOpen(0);
        if (gGameController == NULL)
        {
            std::cout << "Warning: Unable to open game controller! SDL Error: " << SDL_GetError();
        }
    }
    

   // obj.rotate(45, 0, 0);
    
    

    while (cycle() == 0);


    return 0;
}


struct _point_ {
    float x, y, z = 0;
};

struct _line_ {
    _point_ p1, p2;
};



void lines(_line_ line[100], int count) {

    for (int i = 0; i < count; i++)
    {
        line[i].p1.x = -line[i].p1.x / line[i].p1.z;
        line[i].p1.y = -line[i].p1.y / line[i].p1.z;

        line[i].p2.x = -line[i].p2.x / line[i].p2.z;
        line[i].p2.y = -line[i].p2.y / line[i].p2.z;

    }
    // */



    float MINx = 0; //Ax
    float MAXx = 0; //Bx

    float MINy = 0; //Ay
    float MAXy = 0; //By

    //find min and max x and y
    for (int i = 0; i < count; i++)
    {
        if (line[i].p1.x > MAXx)
        {
            MAXx = line[i].p1.x;
        }
        if (line[i].p1.x < MINx)
        {
            MINx = line[i].p1.x;
        }

        if (line[i].p1.y > MAXy)
        {
            MAXy = line[i].p1.y;
        }
        if (line[i].p1.y < MINy)
        {
            MINy = line[i].p1.y;
        }



        if (line[i].p2.x > MAXx)
        {
            MAXx = line[i].p2.x;
        }
        if (line[i].p2.x < MINx)
        {
            MINx = line[i].p2.x;
        }

        if (line[i].p2.y > MAXy)
        {
            MAXy = line[i].p2.y;
        }
        if (line[i].p2.y < MINy)
        {
            MINy = line[i].p2.y;
        }
    }

    float ax = 0;
    float ay = 500;
    float bx = 1000;
    float by = 0;


    float Kx = (bx - ax) / (MAXx - MINx);
    float Ky = (by - ay) / (MAXy - MINy);

    float px = ((ax * MAXx) - (MINx * bx)) / (MAXx - MINx);
    float py = ((ay * MAXy) - (MINy * by)) / (MAXy - MINy);
    // /*
    for (int i = 0; i < count; i++)
    {
        line[i].p1.x = (Kx * line[i].p1.x) + px;
        line[i].p1.y = (Ky * line[i].p1.y) + py;

        line[i].p2.x = Kx * line[i].p2.x + px;
        line[i].p2.y = Ky * line[i].p2.y + py;
    }
    // */



    SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < count; i++)
    {
        SDL_RenderDrawLine(render, line[i].p1.x, line[i].p1.y, line[i].p2.x, line[i].p2.y);
    }

    //std::cout << _tri_.l1.p1.x << _tri_.l1.p1.y << _tri_.l1.p2.x << _tri_.l1.p2.y<<std::endl;



}



_line_  setLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
    _line_ line;
    line.p1.x = x1;
    line.p1.y = y1;
    line.p1.z = z1;

    line.p2.x = x2;
    line.p2.y = y2;
    line.p2.z = z2;
    return line;
}

float map(float in, float min_in, float max_in, float min_out, float max_out)
{
    return min_out + ((max_out - min_out) / (max_in - min_in)) * (in - min_in);
}

_point_ angles;
float mov = 0;
int _i_ = 0;
int cycle() {

    SDL_Event event;




    const Uint8* key = SDL_GetKeyboardState(NULL);

    if (key[SDL_SCANCODE_W])
    {
        angles.x++;
    }

    if (key[SDL_SCANCODE_S])
    {
        angles.x--;
    }

    if (key[SDL_SCANCODE_D])
    {
        angles.y++;
    }

    if (key[SDL_SCANCODE_A])
    {
        angles.y--;
    }
    if (key[SDL_SCANCODE_E])
    {
        angles.z++;
    }
    if (key[SDL_SCANCODE_Q])
    {
        angles.z--;
    }

    if (key[SDL_SCANCODE_UP])
    {
        obj.move(0, 0.1, 0);
    }
    if (key[SDL_SCANCODE_DOWN])
    {
        obj.move(0, -0.1, 0);
    }

    if (key[SDL_SCANCODE_LEFT])
    {
        obj.move(0.1, 0, 0);
    }
    if (key[SDL_SCANCODE_RIGHT])
    {
        obj.move(-0.1, 0, 0);
    }
    if (key[SDL_SCANCODE_Z])
    {
        obj.move(0, 0, 0.1);
    }
    if (key[SDL_SCANCODE_X])
    {
        obj.move(0, 0, -0.1);
    }




    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 1;
        }
        /*
        if (event.type == SDL_JOYAXISMOTION)
        {
            //Motion on controller 0
            if (event.jaxis.which == 0)
            {
                //X axis motion
                if (event.jaxis.axis == 2)
                {
                    angles.x = (event.jaxis.value + 32767) / 182;

                }
                else if (event.jaxis.axis == 1)
                {
                    angles.z = (event.jaxis.value + 32767) / 182;
                }
                else if (event.jaxis.axis == 0)
                {
                    mov = map(event.jaxis.value, -32767, 32767, 0, 21);



                }
            }
        }
        */
    }


   

//    std::cout << mov;
   
    obj.rotate(angles.x, angles.y, angles.z);
    angles.x = 0;
    angles.y = 0;
    angles.z = 0;

    obj.draw(render);

    SDL_RenderPresent(render);
  //  obj.rotate(-angles.x, -angles.y, -angles.z);


    SDL_SetRenderDrawColor(render, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(render);


    return 0;
}
