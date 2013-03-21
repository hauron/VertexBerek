#include "modelling.h"
#include "sound/sound.h"

#include <QDebug>

extern int n_lights;

LIGHT lights[16];
int n_lists = 1;
void readLine(FILE *f, char *string)
{
    do
    {
        fgets(string, 255, f);
    } while ((string[0] == '/') || (string[0] == '\n'));
    return;
}

// ladowanie modeli, podajemy indeks w tablicy modeli (da sie to zrobic lepiej?) oraz nazwe pliku, program sam laduje model
void LoadModelMovementGL( INSTANCE* instance, char* file_dir )
{
    FILE *filein;
    char oneline[255];
    int n; // ilosc podzialow
    float x, y, z; // koncowe przesuniecie x, y, z
    filein = fopen( file_dir, "rt");				// File To Load World Data From
    readLine(filein,oneline);
    sscanf(
        oneline,
        "#%d, (%f, %f, %f) - A[%f, %f, %f], B[%f, %f, %f], %c",
        &n, &x, &y, &z,
        &instance->action_moving.A.x, &instance->action_moving.A.y, &instance->action_moving.A.z,
        &instance->action_moving.B.x, &instance->action_moving.B.y, &instance->action_moving.B.z,
        &instance->action_moving.activation);
    instance->action_moving.x = x;
    instance->action_moving.y = y;
    instance->action_moving.z = z;
    instance->action_moving.n = n;
    instance->action_moving.current = 0;
    instance->action_moving.increasing = 1;

    char sound_dir[64];
    readLine(filein,oneline); sscanf( oneline, "music_file = %s", sound_dir );
    readLine(filein,oneline); sscanf( oneline, "sound_buffer = %d",&(instance->action_moving.sound_buffer) );

    if( !(instance->action_moving.sound_loaded) )
    {
        instance->action_moving.sound_loaded = 1;
        LoadALFile( sound_dir, instance->action_moving.sound_buffer, 0 );
    }
    instance->action_moving.sound_source = -1;
    fclose(filein);
}
// ladowanie swiatla konkretnej instancji
void SetLights()
{
    for( int i = 0; i < n_lights; i++ )
    {
        glDisable( GL_LIGHT0+2+i );
        GLfloat LightModelSpecularPosition[] =
                { lights[i].x,
                  lights[i].y,
                  lights[i].z,
                  lights[i].xyz };
        GLfloat LightModelSpecular[] =
                { lights[i].r,
                  lights[i].g,
                  lights[i].b,
                  lights[i].rgb };
        glLightfv(GL_LIGHT0+2+i, GL_DIFFUSE, LightModelSpecular );
        glLightfv(GL_LIGHT0+2+i, GL_POSITION,LightModelSpecularPosition);			// Position The Light
        glEnable( GL_LIGHT0+2+i );
    }
}
void LoadLightingGL( INSTANCE* instance, char* file_dir )
{
    FILE *filein;
    char oneline[255];
    filein = fopen( file_dir, "rt");				// File To Load World Data From
    readLine(filein,oneline);
    sscanf(
        oneline,
        "pos: (%f, %f, %f, %f), colours: (%f, %f, %f, %f)",
        &(instance->action_lighting.x), &(instance->action_lighting.y), &(instance->action_lighting.z),&(instance->action_lighting.xyz),
        &(instance->action_lighting.r), &(instance->action_lighting.g), &(instance->action_lighting.b),&(instance->action_lighting.rgb));
    instance->action_lighting.n_light = n_lights; // przypisanie indeksu swiatla
    lights[n_lights].x = (instance->action_lighting.x);
    lights[n_lights].y = (instance->action_lighting.y);
    lights[n_lights].z = (instance->action_lighting.z);
    lights[n_lights].xyz = (instance->action_lighting.xyz);
    lights[n_lights].r = (instance->action_lighting.r);
    lights[n_lights].g = (instance->action_lighting.g);
    lights[n_lights].b = (instance->action_lighting.b);
    lights[n_lights].rgb = (instance->action_lighting.rgb);
    /*glDisable( GL_LIGHT0+2+n_lights );
    GLfloat LightModelSpecularPosition[] =
            { instance->action_lighting.x,
                instance->action_lighting.y,
                instance->action_lighting.z,
                instance->action_lighting.xyz };
    GLfloat LightModelSpecular[] =
            { instance->action_lighting.r,
                instance->action_lighting.g,
                instance->action_lighting.b,
                instance->action_lighting.rgb };
    glLightfv(GL_LIGHT0+2+n_lights, GL_SPECULAR, LightModelSpecular );
    glLightfv(GL_LIGHT0+2+n_lights, GL_POSITION,LightModelSpecularPosition);			// Position The Light
    glEnable( GL_LIGHT0+2+n_lights ); */
    n_lights++;
    fclose(filein);
}

void LoadGLModel( MODEL* model, char* file_dir )
{
    float x, y, z, u, v;
    int numtriangles;
    FILE *filein;
    char oneline[255];
    filein = fopen( file_dir, "rt");				// File To Load World Data From

    readLine(filein,oneline);
    sscanf(oneline, "NUMPOLLIES %d\n", &numtriangles);

    model->triangle = new TRIANGLE[numtriangles];
    model->numtriangles = numtriangles;
    for (int loop = 0; loop < numtriangles; loop++)
    {
        for (int vert = 0; vert < 3; vert++)
        {
            readLine(filein,oneline);
            sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
            model->triangle[loop].vertex[vert].x = x;
            model->triangle[loop].vertex[vert].y = y;
            model->triangle[loop].vertex[vert].z = z;
            model->triangle[loop].vertex[vert].u = u;
            model->triangle[loop].vertex[vert].v = v;
        }
        readLine( filein, oneline);
        sscanf( oneline, "%d", &(model->triangle[loop].id_texture) );
    }
    fclose(filein);
    model->prepared = 2; //2 - wymaga ponownego wczytania, tj. zabicia listy i ladowania
    return;
}

void LoadGLSector(Sector* sector, char* file_dir )
{
    FILE *filein;
    char oneline[255];
    char model_dir[256];
    char model_title[256];
    char actions_dir[256];
    int instance_id;
    int model_id;
    float x, y, z, xm, ym, zm, x_angle, y_angle, z_angle; int actions; char c;

    filein = fopen( file_dir, "rt");				// File To Load World Data From
    readLine(filein,oneline);
    //sscanf(oneline, "TITLE %s\n", sector->title);
    //readstr(filein,oneline);
    sscanf(oneline, "NUMMODELS %d\n", &(sector->n_models));
    readLine(filein,oneline);
    sscanf(oneline, "NUMINSTANCES %d\n", &(sector->n_instances));
    for (int loop = 0; loop < sector->n_models; loop++)
    {
        readLine(filein,oneline);
        sscanf(oneline, "#%d - %s %s\n", &model_id, model_dir, model_title);
        sector->models[model_id].title = model_title;
        LoadGLModel( &(sector->models[model_id]), model_dir );
    }
    for (int loop = 0; loop < sector->n_instances; loop++)
    {
        readLine(filein,oneline);
        sscanf(oneline, "#%d - %d, (%f, %f, %f), (%f, %f, %f), (%f, %f, %f), %d, %c\n", &instance_id, &model_id, &x, &y, &z, &xm, &ym, &zm, &x_angle, &y_angle, &z_angle, &actions, &c );
        sector->instances[instance_id].model_id = model_id;
        sector->instances[instance_id].wektor.x = x;
        sector->instances[instance_id].wektor.y = y;
        sector->instances[instance_id].wektor.z = z;
        sector->instances[instance_id].wektor.xm = xm*sector->xm;
        sector->instances[instance_id].wektor.ym = ym*sector->ym;
        sector->instances[instance_id].wektor.zm = zm*sector->zm;
        sector->instances[instance_id].wektor.x_angle = x_angle;
        sector->instances[instance_id].wektor.y_angle = y_angle;
        sector->instances[instance_id].wektor.z_angle = z_angle;
        sector->instances[instance_id].actions = actions;
        sector->instances[instance_id].mode = c;
        if( c == 'm' )
        {
            readLine(filein,oneline);
            sscanf(oneline, "%s", actions_dir );
            LoadModelMovementGL( &(sector->instances[instance_id]), actions_dir );
        }
        if( c == 'l' )
        {
            readLine(filein,oneline);
            sscanf(oneline, "%s", actions_dir );
            LoadLightingGL( &(sector->instances[instance_id]), actions_dir );
        }
    }
    fclose(filein);
}


void PreloadGLSector(Map *map, FILE *filein, int i)
{
    char oneline[256] = "";
    char sector_dir[256] = "";
    char sector_title[256] = "";
    int sector_id = 666;

    readLine(filein, oneline);
    sscanf(oneline,
           "#%d - (%f, %f, %f), (%f, %f, %f), %s %s %lf\n",
                &sector_id,
                &(map->sector[i].x),
                &(map->sector[i].y),
                &(map->sector[i].z),
                &(map->sector[i].xm),
                &(map->sector[i].ym),
                &(map->sector[i].zm),
                sector_dir,
                sector_title,
                &(map->sector[i].rotation));

    strcpy(sector_title, map->sector[sector_id].title);
    LoadGLSector( &(map->sector[i]), sector_dir );
}
void LoadGLMap(Map* map, const char* file_dir)
{
    float mcolor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
    float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
    glMateriali(GL_FRONT, GL_SHININESS, 40);

    n_lights = 0; // reset swiatel
    FILE *filein;
    char oneline[255];
    filein = fopen( file_dir, "r");				// File To Load World Data From

    readLine(filein,oneline);
    sscanf(oneline, "%s\n", (map->title));
    readLine(filein,oneline);
    sscanf(oneline, "NUMSECTORS %d\n", &(map->n_sectors));

    for (int loop = 0; loop < map->n_sectors; loop++)
        PreloadGLSector(map, filein, loop);

    fclose(filein);
    return;
}

void DrawTriangleGL( TRIANGLE* triangle, POINT_XYZ* p, POINT_XYZ* normal )
{
    glBegin(GL_TRIANGLES);
        glNormal3f( normal->x, normal->y, normal->z );

        glTexCoord2f(triangle->vertex[0].u, triangle->vertex[0].v);
        glVertex3f(p[0].x,p[0].y,p[0].z);

        glTexCoord2f(triangle->vertex[1].u, triangle->vertex[1].v);
        glVertex3f(p[1].x,p[1].y,p[1].z);

        glTexCoord2f(triangle->vertex[2].u, triangle->vertex[2].v);
        glVertex3f( p[2].x, p[2].y, p[2].z);
    glEnd();
}

void PrepareGLModel(MODEL* GLModel)
{
    if( GLModel->prepared == 2 )
        glDeleteLists( GLModel->n_list, 1 );
    GLModel->prepared = 1;
    POINT_XYZ p[3];
    GLModel->n_list = n_lists;
    GLfloat u_m, v_m;
    int numtriangles;
    numtriangles = GLModel->numtriangles;
    // obroty! - byc moze zbedne?
    // Process Each Triangle
    glNewList(n_lists++,GL_COMPILE);	// GL_COMPILE - lista jest kompilowana, ale nie wykonywana

    for (int loop_m = 0; loop_m < numtriangles; loop_m++)
    {
        glBindTexture(GL_TEXTURE_2D,
                      GraphicsSystem::instance()->getTexture(GLModel->triangle[loop_m].id_texture)->texID);
        // obliczenie faktycznych wartosci wspolrzednych punktow - niezbedne do obliczenia normalnych (do swiatla)
        p[0].x = (GLModel->triangle[loop_m].vertex[0].x);
        p[0].y = (GLModel->triangle[loop_m].vertex[0].y);
        p[0].z = (GLModel->triangle[loop_m].vertex[0].z);

        p[1].x = (GLModel->triangle[loop_m].vertex[1].x);
        p[1].y = (GLModel->triangle[loop_m].vertex[1].y);
        p[1].z = (GLModel->triangle[loop_m].vertex[1].z);

        p[2].x = (GLModel->triangle[loop_m].vertex[2].x);
        p[2].y = (GLModel->triangle[loop_m].vertex[2].y);
        p[2].z = (GLModel->triangle[loop_m].vertex[2].z);

        // odbicie wspolrzednych... z jakiegos powodu swiat byl odbiciem lustrzanym dla trzech osi XYZ

        glBegin(GL_TRIANGLES);
            POINT_XYZ* normal = ComputeNormalGL( p );
            glNormal3f( normal->x, normal->y, normal->z );
            u_m = GLModel->triangle[loop_m].vertex[0].u;
            v_m = GLModel->triangle[loop_m].vertex[0].v;
            glTexCoord2f(u_m,v_m); glVertex3f(p[0].x,p[0].y,p[0].z);


            u_m = GLModel->triangle[loop_m].vertex[1].u;
            v_m = GLModel->triangle[loop_m].vertex[1].v;
            glTexCoord2f(u_m,v_m); glVertex3f(p[1].x,p[1].y,p[1].z);

            u_m = GLModel->triangle[loop_m].vertex[2].u;
            v_m = GLModel->triangle[loop_m].vertex[2].v;
            glTexCoord2f(u_m,v_m); glVertex3f( p[2].x, p[2].y, p[2].z);
        glEnd();
    }

    glEndList();

}

int DrawGLModel(MODEL* GLModel, MOVEMENT vector) // funkcja rysujaca poszczegolne modele
{
    if( GLModel->prepared != 1 )
        PrepareGLModel( GLModel );

    // obroty! - byc moze zbedne?
    RotateGL( vector.x_angle, vector.y_angle, vector.z_angle );
    glScaled(vector.xm, vector.ym, vector.zm );
    glTranslatef(vector.x, vector.y, vector.z );
        glCallList(GLModel->n_list);
    glTranslatef(-vector.x, -vector.y, -vector.z );
    glScaled(1/vector.xm, 1/vector.ym, 1/vector.zm );
    RotateGL( -vector.x_angle, -vector.y_angle, -vector.z_angle );

    return true;
}

int DrawGLSector(Sector* sector)
{
    glRotated( sector->rotation, 0.0f, 1.0f, 0.0f );
    for( int i = 0; i < sector->n_instances; i++ )
    {
        if( !(sector->instances[i].actions) )
        {
            if( sector->instances[i].mode == 'm' )
            {
                MOVEMENT wektor = MoveGL( &(sector->instances[i]) );
                sector->instances[i].wektor.xm = 0;
                DrawGLModel
                    (
                        &(sector->models[sector->instances[i].model_id]),
                        wektor
                    );
            }
        }
        else
        {
            DrawGLModel( &(sector->models[sector->instances[i].model_id]), sector->instances[i].wektor);
        }
    }
    glRotated( -(sector->rotation), 0.0f, 1.0f, 0.0f );
    return true;
}

void DrawGLMap(Map *map)
{

    for( int i = 0; i < map->n_sectors; i++ )
    {
        glTranslatef((map->sector[i]).x,map->sector[i].y,map->sector[i].z);
        DrawGLSector( &(map->sector[i]) );
        glTranslatef(-map->sector[i].x,-map->sector[i].y,-map->sector[i].z);
    }
}


POINT_XYZ* NormalizeNormalGL(POINT_XYZ* v)
{
    // calculate the length of the vector
    float len = (float)(sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z)));

    // avoid division by 0
    if (len == 0.0f)
        len = 1.0f;

    // reduce to unit size
    v->x /= len;
    v->y /= len;
    v->z /= len;
    return v;
}

POINT_XYZ* ComputeNormalGL(POINT_XYZ* v)
{
    POINT_XYZ a, b;

    // calculate the vectors A and B
    // note that v[3] is defined with counterclockwise winding in mind
    // a
    a.x = v[0].x - v[1].x;
    a.y = v[0].y - v[1].y;
    a.z = v[0].z - v[1].z;
    // b
    b.x = v[1].x - v[2].x;
    b.y = v[1].y - v[2].y;
    b.z = v[1].z - v[2].z;

    // calculate the cross product and place the resulting vector
    // into the address specified by vertex_t *normal
    POINT_XYZ normal;
    normal.x = (a.y * b.z) - (a.z * b.y);
    normal.y = (a.z * b.x) - (a.x * b.z);
    normal.z = (a.x * b.y) - (a.y * b.x);

    // normalize
    return NormalizeNormalGL(&normal);
}



//#include <QString>
//#include <QTextStream>
//#include <QFile>

//void PreloadGLSector(MAP *map, QString data, int i)
//{
//    char sector_dir[256];
//    char sector_title[256];
//    int sector_id;

//    QTextStream s(data.toAscii());
//    qDebug() << "mapLine" << data;

//    //    std::stringstream s(oneline);

//        s   >> sector_id
//            >> map->sector[i].x
//            >> map->sector[i].y
//            >> map->sector[i].z
//            >> map->sector[i].xm
//            >> map->sector[i].ym
//            >> map->sector[i].zm
//            >> sector_dir
//            >> sector_title
//            >> map->sector[i].rotation;


//        qDebug() << "\n=============\nmapLine" << data << "into:\n"
//                        << sector_id
//                        << map->sector[i].x
//                        << map->sector[i].y
//                        << map->sector[i].z
//                        << map->sector[i].xm
//                        << map->sector[i].ym
//                        << map->sector[i].zm
//                        << sector_dir
//                        << sector_title
//                        << map->sector[i].rotation;


////        sscanf(oneline,
////               "#%d - (%f, %f, %f), (%f, %f, %f), %s %s %lf\n",
////                &sector_id,
////                &(map->sector[i].x),
////                &(map->sector[i].y),
////                &(map->sector[i].z),
////                &(map->sector[i].xm),
////                &(map->sector[i].ym),
////                &(map->sector[i].zm),
////                sector_dir,
////                sector_title,
////                &(map->sector[i].rotation));


//    map->sector[sector_id].title = sector_title;
//   // LoadGLSector( &(map->sector[loop]), sector_dir );



//}

//#define COMMENT_SIGN    '$'
//bool isValidLine(QString line)
//{
//    return (!(line.isEmpty() || line[0] == COMMENT_SIGN));
//}

//void LoadGLMap( MAP* map, char* file_dir )
//{
//    float mcolor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
//    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
//    float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
//    glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
//    glMateriali(GL_FRONT, GL_SHININESS, 40);

//    n_lights = 0; // reset swiatel


//    // Open file
//    QFile file(file_dir);
//    if(!file.open(QIODevice::ReadOnly))
//        return;
//    QTextStream in(&file);

//    // Prepare data
//    QStringList dataList;

//    // Read all lines
//    QString line;
//    while(!in.atEnd())
//    {
//        line = in.readLine();
//        if(isValidLine(line))
//            dataList << line;
//    }
//    file.close();

//    map->title = dataList.first().toStdString();
//    map->n_sectors = dataList.at(1).toInt();

//    for (int loop = 0; loop < map->n_sectors; loop++)
//        PreloadGLSector(map, dataList.at(loop+2), loop);


//    return;
//}
