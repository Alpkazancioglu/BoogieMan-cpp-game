#version 330

// Input vertex attributes
in vec3 vertexPosition;

// Input uniform values
uniform mat4 matProjection;
uniform mat4 matModel;
uniform mat4 matView;

uniform float rotationAngle;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;

void main()
{
    
    fragPosition = vertexPosition;

    float angle = radians(rotationAngle); 
    angle = angle / 30;

    mat4 rotationMatrix = mat4(
     1.0,0.0 , 0.0, 0.0,
     0.0, cos(angle), -sin(angle), 0.0,
     0.0, sin(angle),cos(angle), 0.0,
     0.0, 0.0, 0.0, 1.0
    );

    // Remove translation from the view matrix
    mat4 rotView = mat4(mat3(matView));
    vec4 clipPos = matProjection*rotationMatrix*rotView*vec4(vertexPosition, 1.0);

    // Calculate final vertex position
    gl_Position = clipPos;
}
