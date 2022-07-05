class iluminacao {
    float mat_specular[4];
    float mat_shininess;
    float light_position[4];
    float light_diffuse[4];
public:
    iluminacao();
    void light_on();
    void render_light();
    void light_off();
};
