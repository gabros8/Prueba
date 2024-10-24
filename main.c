#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "matrix.h"
#include "mesh.h"

#define N_POINTS (62)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t cube_rotation = {0, 0, 0};
vec3_t cube_translation = {0, 0, 0};
vec3_t cube_scale = {1, 1, 1};

float fov_factor = 720.0f;

bool is_running = false;
int previous_frame_time = 0;

void setup(void){
    // Allocate the required memory in bytes to hold the color buffer
    color_buffer = (uint32_t*) malloc(sizeof (uint32_t) * window_width * window_height);

    if(!color_buffer){
        fprintf(stderr, "Error allocating memory for frame buffer.\n");
    }

    int point_count = 0;
    load_obj_file_data("flecha.obj");
    // Start loading my array of vectors
    // From -1 to 1 (in this 9*9*9 cube)
    /*for (float x=-1; x<=1; x+=0.25){
        for (float y=-1; y<=1; y+=0.25){
            for (float z=-1; z<=1; z+=0.25){
                vec3_t new_point = {.x = x, .y = y, .z = z};
                // vec3_t new_point = {x, y, z};
                cube_points[point_count++] = new_point;
            }
        }
    }*/

}

void process_input(void){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
            break;
    }
}

/*
 * Function that receives a 3D point and returns a projected 2D point
 */
vec2_t project(vec3_t point){
    vec2_t vec2 = {point.x*fov_factor/point.z, point.y*fov_factor/point.z};
    return vec2;
    // Implementar las dos tipos de proyección que vimos en clase,
    // Probar con ambos tipos de proyección.
    // Considerar que sólo una proyección se ejecuta, la otra queda comentada
}

void update(void){
    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    cube_scale.x = 0.1;
    cube_scale.y = 0.1;
    cube_scale.z = 0.1;

    //cube_translation.x += 0.10f;
    cube_translation.z = 5.0f;

    // Create scale, rotation, and translation matrices that will be used to multiply the mesh vertices
    mat4_t scale_matrix = mat4_make_scale(cube_scale.x,
                                          cube_scale.y,
                                          cube_scale.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(cube_rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(cube_rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(cube_rotation.z);
    mat4_t translation_matrix = mat4_make_translation(
            cube_translation.x, cube_translation.y,
            cube_translation.z);

    // Create a World Matrix combining scale, rotation, and translation matrices
    mat4_t world_matrix = mat4_identity();

    // Order matters: First scale, then rotate, then translate.
    // [T]*[R]*[S]*v
    world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
    world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

    vec4_t transformed_points[N_POINTS];


    for(int i = 0; i < vertices; i++){

        vec4_t transformed_point = vec4_from_vec3(mesh.vertices[i]);

        // Multiply the world matrix by the original vector
        transformed_point = mat4_mul_vec4(world_matrix,
                                          transformed_point);

        // Save transformed vertex in the array of transformed vertices
        //transformed_points[i] = transformed_point;

        // Project the current point
        vec2_t projected_point = project(
                vec3_from_vec4(transformed_point));

        // Save the projected 2D vectors
        projected_points[i] = projected_point;
    }

    for(int i = 0; i < caras ; i++) {
        draw_triangle(projected_points[mesh.faces[i].a-1].x,
            projected_points[mesh.faces[i].a-1].y,
            projected_points[mesh.faces[i].b-1].x,
            projected_points[mesh.faces[i].b-1].y,
            projected_points[mesh.faces[i].c-1].x,
            projected_points[mesh.faces[i].c-1].y,
            0xFFFFFF00);
    }
}

void render(void){


    // Loop all the projected points and render them
    /*for(int i=0; i<8; i++){
        vec2_t projected_point = projected_points[i];

        draw_pixel(
                projected_point.x + window_width/2,
                projected_point.y + window_height/2,
                0xFFFFFF00
                );
    }*/
    //draw_grid();
    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    is_running = initialize_window();

    setup();

    while (is_running){
	    previous_frame_time = SDL_GetTicks();

        process_input();
        update();
        render();

	// Guarantees that every frame is executed every FRAME_TARGET_TIME
        // Wait some time until the reach the target frame time in milliseconds
        int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
        // Only delay execution if we are running too fast
        if(time_to_wait >0 && time_to_wait <= FRAME_TARGET_TIME){
            SDL_Delay(time_to_wait);
        }

    }

    destroy_window();
    return 0;
}

void draw_line(int x0, int y0, int x1, int y1, color_t color) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int sx = (dx >= 0) ? 1 : -1;  // Dirección en x
    int sy = (dy >= 0) ? 1 : -1;  // Dirección en y

    dx = abs(dx);
    dy = abs(dy);

    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    while (1) {
        draw_pixel(x0 + window_width/2, y0 + window_height/2, color);  // Coloca el punto actual

        if (x0 == x1 && y0 == y1) {
            break;  // Si llegamos al punto final, terminamos
        }

        e2 = err;

        if (e2 > -dx) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }
}