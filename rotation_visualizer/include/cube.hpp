float texture_cube[] = {
    // position         // texture coordinates
    // front face
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,   // top left
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top right
    -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,  // bottom left
    -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,  // bottom left
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f,   // bottom right
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top right

    // back face
    -0.5f, 0.5f, -0.5, 0.0f, 0.0f,   // top left
    0.5f, 0.5f, -0.5, 1.0f, 0.0f,    // top right
    -0.5f, -0.5f, -0.5, 0.0f, 1.0f,  // bottom left
    -0.5f, -0.5f, -0.5, 0.0f, 1.0f,  // bottom left
    0.5f, -0.5f, -0.5, 1.0f, 1.0f,   // bottom right
    0.5f, 0.5f, -0.5, 1.0f, 0.0f,    // top right

    // left face
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,    // front top
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // front bottom
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // back top
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // back top
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // back bottom
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // front bottom

    // right face
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f,    // front top
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // front bottom
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // back top
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // back top
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // back bottom
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // front bottom

    // top face
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f,    // front left
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // front right
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // back left
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // back left
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // back right
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // front right

    // bottom face
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,    // front left
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // front right
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   // back left
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   // back left
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // back right
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // front right
};

float cube[] = {
    // position
    // front face
    -0.5f, 0.5f, 0.5f,   // top left
    0.5f, 0.5f, 0.5f,    // top right
    -0.5f, -0.5f, 0.5f,  // bottom left
    -0.5f, -0.5f, 0.5f,  // bottom left
    0.5f, -0.5f, 0.5f,   // bottom right
    0.5f, 0.5f, 0.5f,    // top right

    // back face
    -0.5f, 0.5f, -0.5,   // top left
    0.5f, 0.5f, -0.5,    // top right
    -0.5f, -0.5f, -0.5,  // bottom left
    -0.5f, -0.5f, -0.5,  // bottom left
    0.5f, -0.5f, -0.5,   // bottom right
    0.5f, 0.5f, -0.5,    // top right

    // left face
    -0.5f, 0.5f, 0.5f,    // front top
    -0.5f, -0.5f, 0.5f,   // front bottom
    -0.5f, 0.5f, -0.5f,   // back top
    -0.5f, 0.5f, -0.5f,   // back top
    -0.5f, -0.5f, -0.5f,  // back bottom
    -0.5f, -0.5f, 0.5f,   // front bottom

    // right face
    0.5f, 0.5f, 0.5f,    // front top
    0.5f, -0.5f, 0.5f,   // front bottom
    0.5f, 0.5f, -0.5f,   // back top
    0.5f, 0.5f, -0.5f,   // back top
    0.5f, -0.5f, -0.5f,  // back bottom
    0.5f, -0.5f, 0.5f,   // front bottom

    // top face
    0.5f, 0.5f, 0.5f,    // front left
    -0.5f, 0.5f, 0.5f,   // front right
    0.5f, 0.5f, -0.5f,   // back left
    0.5f, 0.5f, -0.5f,   // back left
    -0.5f, 0.5f, -0.5f,  // back right
    -0.5f, 0.5f, 0.5f,   // front right

    // bottom face
    0.5f, -0.5f, 0.5f,    // front left
    -0.5f, -0.5f, 0.5f,   // front right
    0.5f, -0.5f, -0.5f,   // back left
    0.5f, -0.5f, -0.5f,   // back left
    -0.5f, -0.5f, -0.5f,  // back right
    -0.5f, -0.5f, 0.5f,   // front right
};

float tex_norm_cube[] = {
    // position         // normals         // texture
    // front face
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // top left
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,    // top right
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom left
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // bottom right
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,    // top right

    // back face
    -0.5f, 0.5f, -0.5, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,   // top left
    0.5f, 0.5f, -0.5, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,    // top right
    -0.5f, -0.5f, -0.5, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // bottom left
    -0.5f, -0.5f, -0.5, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // bottom left
    0.5f, -0.5f, -0.5, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // bottom right
    0.5f, 0.5f, -0.5, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,    // top right

    // left face
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // front top
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // front bottom
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // back top
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // back top
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // back bottom
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // front bottom

    // right face
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // front top
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // front bottom
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // back top
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // back top
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // back bottom
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // front bottom

    // top face
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,    // front left
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // front right
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // back left
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // back left
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // back right
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // front right

    // bottom face
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,    // front left
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // front right
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,   // back left
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,   // back left
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // back right
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // front right
};

float cube_with_normals[] = {
    // position         // normals
    // front face
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   // top left
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,    // top right
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // bottom left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // bottom left
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   // bottom right
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,    // top right

    // back face
    -0.5f, 0.5f, -0.5, 0.0f, 0.0f, -1.0f,   // top left
    0.5f, 0.5f, -0.5, 0.0f, 0.0f, -1.0f,    // top right
    -0.5f, -0.5f, -0.5, 0.0f, 0.0f, -1.0f,  // bottom left
    -0.5f, -0.5f, -0.5, 0.0f, 0.0f, -1.0f,  // bottom left
    0.5f, -0.5f, -0.5, 0.0f, 0.0f, -1.0f,   // bottom right
    0.5f, 0.5f, -0.5, 0.0f, 0.0f, -1.0f,    // top right

    // left face
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,    // front top
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,   // front bottom
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,   // back top
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,   // back top
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  // back bottom
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,   // front bottom

    // right face
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,    // front top
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,   // front bottom
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,   // back top
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,   // back top
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // back bottom
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,   // front bottom

    // top face
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,    // front left
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // front right
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // back left
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // back left
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // back right
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // front right

    // bottom face
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,    // front left
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,   // front right
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,   // back left
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,   // back left
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  // back right
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,   // front right
};