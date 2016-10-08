/*
    Superformula Shape Animations

    Schulc Attila

    You must have the SDL2 library installed. (sudo apt-get install libsdl2-dev)

    Compile using:
    "g++ -std=c++11 superformula.cpp -o superformula -lSDL2"

    Superformula equation: https://en.wikipedia.org/wiki/Superformula
*/

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>


const static int WIDTH = 500;
const static int HEIGHT = 500;
const static int scale = 100;
const double speed = 0.1;

void usage()
{
    std::cout << "superformula.cpp usage:\n"
              << "\t -t [integer]\t\tAnimation interval (default: 1000)\n"
              << "\t -d [integer]\t\tDelay between frames in miliseconds (default: 30)\n"
              << "\t -s [double]\t\tSmooth step (default: 0.001)\n"
              << "\t -r [0 or 1]\t\tRandom shapes (default: 0)\n"
              << "\t -h          \t\tPrint this help\n";
}



double superformula(double theta, double a, double b, double m, double n1, double n2, double n3)
{
    // fabs() not abs() !!!
    return (1 / pow(pow(fabs(cos(m * theta / 4.0) / a), n2) + pow(fabs(sin(m * theta / 4.0) / b), n3), n1));
}



int main( int argc, char* argv[] )
{
    srand(time(0));

    bool running = false;

    int interval = 1000;
    int delay = 50;
    double smooth_step = 0.005;
    bool random_shapes = 0;

    usage();


    if (argc > 1) {
        for (int i=1; i<argc; i++)
        {
            //std::cout << i << ": " << argv[i] << "  " << argv[i+1] << std::endl;
            if(argv[i] == std::string("-t"))
                interval = atoi(argv[++i]);
            else if (argv[i] == std::string("-d"))
                delay = atoi(argv[++i]);
            else if (argv[i] == std::string("-s"))
                smooth_step = atof(argv[++i]);
            else if (argv[i] == std::string("-r"))
                random_shapes = atoi(argv[++i]);
            else if (argv[i] == std::string("-h"))
            {
                usage();
                exit(0);
            }
            else {
                std::cerr << "Error: Command-line argument " << argv[i] << " not recognized.\n";
                exit(-1);
            }
        }
    }

	//Start SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

    // Creating a window
	SDL_Window *win = SDL_CreateWindow("Superformula", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}

    // Creating renderer
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
    }

    running = true;

    int num_of_points = ceil(2 * M_PI / smooth_step);
    std::cout << "Number of points: " << num_of_points << std::endl;

    SDL_Point* fig1 = new SDL_Point[num_of_points];
    SDL_Point* fig2 = new SDL_Point[num_of_points];
    SDL_Point* fig3 = new SDL_Point[num_of_points];
    SDL_Point* fig4 = new SDL_Point[num_of_points];

    int m[4] = {2, 4, 10, 30};
    if (random_shapes)
    {
        for (int i=0; i<4; i++)
            m[i] = rand()%50;
    }


    SDL_Event ev;
    int t = 0;

    while (t < interval && running)
    {
        while ( SDL_PollEvent(&ev) )
		{
            if (ev.type == SDL_QUIT)
                running = false;
            else if (ev.type == SDL_KEYDOWN) {
				if ( ev.key.keysym.sym == SDLK_ESCAPE )
				running = false;
            }
		}
        //First clear the renderer
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 1);
        SDL_RenderClear(ren);

        int index = 0;

        for (double theta_i = 0.0; theta_i < 2 * M_PI; theta_i = theta_i+smooth_step)
        {

            double rad = superformula(theta_i, 1.0, 1.0, m[0], 1.0, sin(double(t)*speed), cos(double(t)*speed));
            fig1[index].x = rad * cos(theta_i) * scale + WIDTH*0.25;
            fig1[index].y = rad * sin(theta_i) * scale + HEIGHT*0.25;

            rad = superformula(theta_i, 1.0, 1.0, m[1], 1.0, sin(double(t)*speed), cos(double(t)*speed));
            fig2[index].x = rad * cos(theta_i) * scale + WIDTH*0.75;
            fig2[index].y = rad * sin(theta_i) * scale + HEIGHT*0.25;

            rad = superformula(theta_i, 1.0, 1.0, m[2], 1.0, sin(double(t)*speed), cos(double(t)*speed));
            fig3[index].x = rad * cos(theta_i) * scale + WIDTH*0.25;
            fig3[index].y = rad * sin(theta_i) * scale + HEIGHT*0.75;

            rad = superformula(theta_i, 1.0, 1.0, m[3], 1.0, sin(double(t)*speed), cos(double(t)*speed));
            fig4[index].x = rad * cos(theta_i) * scale + WIDTH*0.75;
            fig4[index].y = rad * sin(theta_i) * scale + HEIGHT*0.75;

            index++;
        }

        SDL_SetRenderDrawColor(ren, 255, 0, 0, 1);
        SDL_RenderDrawLines(ren, fig1, num_of_points);
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 1);
        SDL_RenderDrawLines(ren, fig2, num_of_points);
        SDL_SetRenderDrawColor(ren, 0, 0, 255, 1);
        SDL_RenderDrawLines(ren, fig3, num_of_points);
        SDL_SetRenderDrawColor(ren, 255, 200, 0, 1);
        SDL_RenderDrawLines(ren, fig4, num_of_points);


        SDL_RenderPresent(ren);

        SDL_Delay(delay);
        t++;
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

	return 0;
}
