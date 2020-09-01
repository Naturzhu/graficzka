#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 light;

//Attributes
in vec4 vertex; //Vertex coordinates in model space
in vec4 color;

//zmienne interpolowane
out vec4 i_color;

void main(void) {

    float d;
    float maxD = 20;
    float minD = 10;

    d = distance(M*vertex, light);
    d = 1 - (d-minD)/(maxD-minD);
    
    i_color = vec4(color.rgb*d, color.a);
    gl_Position=P*V*M*vertex;
}