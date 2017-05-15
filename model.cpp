#include "model.hpp"
#include <sstream>
#include <fstream>

using std::istringstream;
using std::ifstream;
using std::string;

mat3 model::get_verts(const face& face) const
{
    return mat3(verts[face.vi[0]], verts[face.vi[1]], verts[face.vi[2]]);
}

std::unique_ptr<model> load_model(const string& file)
{
    const static string VERTEX_KEY("v");
    const static string VERTEX_NORM_KEY("vn");
    const static string VERTEX_TEXTURE_KEY("vt");
    const static string FACE_KEY("f");

    auto model_ptr = std::make_unique<model>();
    ifstream input_stream(file);
    string s;

    // load data
    while (getline(input_stream, s))
    {
        istringstream line(s);
        string linetype;
        line >> linetype;
        if (linetype == VERTEX_KEY)
        {
            float x, y, z;
            line >> x >> y >> z;
            model_ptr->verts.push_back(glm::vec3(x, y, z));
        }
        else if (linetype == VERTEX_NORM_KEY)
        {
            float x, y, z;
            line >> x >> y >> z;
            model_ptr->vert_norms.push_back(glm::vec3(x, y, z));
        }
        else if (linetype == VERTEX_TEXTURE_KEY)
        {
            float u, v;
            line >> u >> v;
            model_ptr->text_verts.push_back(glm::vec2(u, v));
        }
        else if(linetype == FACE_KEY)
        {
            face fdata;
            for (int i = 0; i < 3; ++i)
            {
                string vertex_string;
                line >> vertex_string;
                istringstream vss(vertex_string);

                string vertex_index;
                getline(vss, vertex_index, '/');
                fdata.vi[i] = stoi(vertex_index) - 1;

                string vertex_texture_index;
                if (! getline(vss, vertex_texture_index, '/')) { continue; }
                if (vertex_texture_index.length() > 0)
                {
                    fdata.vti[i] = stoi(vertex_texture_index) - 1;
                }

                string vertex_normal_index;
                if (! getline(vss, vertex_normal_index, '/')) { continue; }
                fdata.vni[i] = stoi(vertex_normal_index) - 1;
            }

            model_ptr->faces.push_back(fdata);
        }
    }

    // use data to construct face
    return model_ptr;
}

void test_print_model(const model& m)
{
    printf("model has %lu verts and %lu faces\n\n",
            m.verts.size(), m.faces.size());
    for (auto& face : m.faces)
    {
        int vi1 = face.vi.x;
        int vi2 = face.vi.y;
        int vi3 = face.vi.z;
        glm::vec3 v1 = m.verts[vi1];
        glm::vec3 v2 = m.verts[vi2];
        glm::vec3 v3 = m.verts[vi3];
        printf("%d - %f, %f, %f\n", vi1, v1.x, v1.y, v1.z);
        printf("%d - %f, %f, %f\n", vi2, v2.x, v2.y, v2.z);
        printf("%d - %f, %f, %f\n", vi3, v3.x, v3.y, v3.z);
        printf("\n");
    }
}
