#include "mainsystem.h"

MainSystem::MainSystem(QObject *parent) :
    QObject(parent)
{
    if(!World::instance()->initializeMap("data/map_zewnatrz.txt"))
        exitRequested();

    if(!World::instance()->initializePlayer())
        exitRequested();

    connect(m_window.getCentralWidget(), SIGNAL(keyPressed(int)),       this, SLOT(keyPressed(int)));
    connect(m_window.getCentralWidget(), SIGNAL(keyReleased(int)),      this, SLOT(keyReleased(int)));
    connect(m_window.getCentralWidget(), SIGNAL(mouseMoved(int, int)),  this, SLOT(mouseMoved(int, int)));

    connect(&fpsTimer, SIGNAL(timeout()), this, SLOT(frameTick()));
    fpsTimer.start(25);

    m_window.show();
}

MainSystem::~MainSystem()
{
}

void MainSystem::keyPressed(int eventKey)
{
    qDebug() << "key pressed" << eventKey;
    switch(eventKey)
    {
    /* MOVEMENT - STRAFING */
    case Qt::Key_W: World::instance()->getPlayer()->physics().setMovingForward(true); break;
    case Qt::Key_S: World::instance()->getPlayer()->physics().setMovingBackward(true); break;

    case Qt::Key_A: World::instance()->getPlayer()->physics().setMovingLeft(true); break;
    case Qt::Key_D: World::instance()->getPlayer()->physics().setMovingRight(true); break;

    /* MOVEMENT - UP/DOWN*/
    case Qt::Key_R: World::instance()->getPlayer()->physics().setMovingUp(true); break;
    case Qt::Key_F: World::instance()->getPlayer()->physics().setMovingDown(true); break;

    /* ROTATION */
    case Qt::Key_Q: World::instance()->getPlayer()->physics().setTurningLeft(true); break;
    case Qt::Key_E: World::instance()->getPlayer()->physics().setTurningRight(true); break;

    /* DATA CONTROL */
    case Qt::Key_F5:        // reload map from files
        if(!World::instance()->initializeMap("data/map_zewnatrz.txt"))
            exitRequested();
        break;

    /* GLOBAL CONTROL */
    case Qt::Key_F1:        // fullscreen on/off
        m_window.toggleFullScreen();
        break;

    case Qt::Key_Escape:
        exitRequested();
    }
}

void MainSystem::keyReleased(int eventKey)
{
    qDebug() << "key released" << eventKey;
    switch(eventKey)
    {
    /* MOVEMENT - STRAFING */
    case Qt::Key_W: World::instance()->getPlayer()->physics().setMovingForward(false); break;
    case Qt::Key_S: World::instance()->getPlayer()->physics().setMovingBackward(false); break;

    case Qt::Key_A: World::instance()->getPlayer()->physics().setMovingLeft(false); break;
    case Qt::Key_D: World::instance()->getPlayer()->physics().setMovingRight(false); break;

    /* MOVEMENT - UP/DOWN*/
    case Qt::Key_R: World::instance()->getPlayer()->physics().setMovingUp(false); break;
    case Qt::Key_F: World::instance()->getPlayer()->physics().setMovingDown(false); break;

    /* ROTATION */
    case Qt::Key_Q: World::instance()->getPlayer()->physics().setTurningLeft(false); break;
    case Qt::Key_E: World::instance()->getPlayer()->physics().setTurningRight(false); break;
    }
}

void MainSystem::mouseMoved(int x, int y)
{
    static int x_old = 0, y_old = 0;

    int xm = x_old - x;
    int ym = y_old - y;

    x_old = x;
    y_old = y;

    float sensitivity = 0.01f;
    Position &position = World::instance()->getPlayer()->position();

    position.angleHorizontal -= xm * sensitivity;
    position.lx = sin(position.angleHorizontal);
    position.lz = -cos(position.angleHorizontal);


    position.angleVertical -= ym * sensitivity;
    position.ly = sin(position.angleVertical);
}

void MainSystem::frameTick()
{
    // here the actual time should be computed! TODO
    float timePassed = 25;

    PhysicsUtils::move(World::instance()->getPlayer()->position(),
                       World::instance()->getPlayer()->physics(),
                       timePassed);

    m_window.getCentralWidget()->updateGL();
}

void MainSystem::exitRequested()
{
    exit(0);
}

#ifdef DO_PRZEJRZENIA

/* CZESC SWITCHA */
//    /* LIGHTNING */
//    case Qt::Key_L:         // light on/off
//        lp=TRUE;				// lp Becomes TRUE
//        light=!light;				// Toggle Light TRUE/FALSE

//        if (!light)				// If Not Light
//        {
//            glDisable(GL_LIGHTING);		// Disable Lighting
//        }
//        else					// Otherwise
//        {
//            glEnable(GL_LIGHTING);		// Enable Lighting
//        }
//        break;
//    case Qt::Key_N:         // night mode on/off
//        if( np == 0 )
//        {
//            np = 1;
//            if( night == 0 )
//            {
//                float mcolor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
//                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
//                glDisable(GL_LIGHT0);
//                GLfloat LightAmbientGlobal[] = { 0.02, 0.02, 0.022, 1.0 };
//                GLfloat LightAmbientGlobalPosition[]= { 0, 10, 0, 1.0f };				 // Light Position ( NEW )
//                glLightfv( GL_LIGHT0, GL_AMBIENT, LightAmbientGlobal );
//                glEnable(GL_LIGHT0);
//                night = 1;
//                for( int i = 0; i < n_lights; i++ )
//                    glEnable(GL_LIGHT0+i+2);
//            }
//            else
//            {
//                float mcolor[] = { 0.6f, 0.6f, 0.6f, 1.0f };
//                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
//                night = 0;
//                glDisable(GL_LIGHT0);
//                GLfloat LightAmbientGlobal[] = { 0.9f, 0.9f, 0.9f, 1.0f };
//                GLfloat LightAmbientGlobalPosition[]= { 0, 10, 0, 1.0f };				 // Light Position ( NEW )
//                glLightfv( GL_LIGHT0, GL_AMBIENT, LightAmbientGlobal );
//                glEnable(GL_LIGHT0);
//                for( int i = 0; i < n_lights; i++ )
//                    glDisable(GL_LIGHT0+i+2);
//            }
//        }
//        break;
//    case Qt::Key_I:         // night mode on/off
//        GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				 // Diffuse Light Values ( NEW )
//        //GLfloat LightPosition[]= { x_light, y_light, z_light, 1.0f };				 // Light Position ( NEW )
//        GLfloat LightPosition[]= { xpos, ytrans, zpos, 1.0f };				 // Light Position ( NEW )
//        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
//        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
//        glEnable(GL_LIGHT1);
//        break;

//    /* GRAPHICS CONTROL */
//    case Qt::Key_B:         // translucency on/off (przenikanie)
//        break;


        //    /* SOUND */
        //    case Qt::Key_X:
        //        break;

        //    /* UNKNOWN - TODO! */
        //    case Qt::Key_V:
        //        fp=TRUE;
        //        filter+=1;
        //        if (filter>2)
        //        {
        //            filter=0;
        //        }
        //        break;

/* CZESC SWITCHA - KONIEC */


void MainSystem::keyPressed(int eventKey)
{
    float fraction = 0.1f;
    switch(eventKey)
    {
    /* MOVEMENT - STRAFING */
    case Qt::Key_D:
        World::instance()->getPlayer()->getPhysics().movingRight(true);
        World::instance()->getPlayer()->x -= World::instance()->getPlayer()->lz * fraction;
        World::instance()->getPlayer()->z += World::instance()->getPlayer()->lx * fraction;
        break;
    case Qt::Key_A:
        World::instance()->getPlayer()->x += World::instance()->getPlayer()->lz * fraction;
        World::instance()->getPlayer()->z -= World::instance()->getPlayer()->lx * fraction;

        break;
    case Qt::Key_W:
        World::instance()->getPlayer()->x += World::instance()->getPlayer()->lx * fraction;
        World::instance()->getPlayer()->z += World::instance()->getPlayer()->lz * fraction;
        break;
    case Qt::Key_S:
        World::instance()->getPlayer()->x -= World::instance()->getPlayer()->lx * fraction;
        World::instance()->getPlayer()->z -= World::instance()->getPlayer()->lz * fraction;
        break;

    /* MOVEMENT - UP/DOWN*/
    case Qt::Key_R:
        World::instance()->getPlayer()->y += mov_speed;
        break;
    case Qt::Key_F:
        World::instance()->getPlayer()->y -= mov_speed;
        break;

    /* ROTATION */
    case Qt::Key_E:
        World::instance()->getPlayer()->angle += 0.02f;
        World::instance()->getPlayer()->lx = sin(World::instance()->getPlayer()->angle);
        World::instance()->getPlayer()->lz = -cos(World::instance()->getPlayer()->angle);
        break;
    case Qt::Key_Q:
        World::instance()->getPlayer()->angle -= 0.02f;
        World::instance()->getPlayer()->lx = sin(World::instance()->getPlayer()->angle);
        World::instance()->getPlayer()->lz = -cos(World::instance()->getPlayer()->angle);
        break;

//    /* LIGHTNING */
//    case Qt::Key_L:         // light on/off
//        lp=TRUE;				// lp Becomes TRUE
//        light=!light;				// Toggle Light TRUE/FALSE

//        if (!light)				// If Not Light
//        {
//            glDisable(GL_LIGHTING);		// Disable Lighting
//        }
//        else					// Otherwise
//        {
//            glEnable(GL_LIGHTING);		// Enable Lighting
//        }
//        break;
//    case Qt::Key_N:         // night mode on/off
//        if( np == 0 )
//        {
//            np = 1;
//            if( night == 0 )
//            {
//                float mcolor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
//                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
//                glDisable(GL_LIGHT0);
//                GLfloat LightAmbientGlobal[] = { 0.02, 0.02, 0.022, 1.0 };
//                GLfloat LightAmbientGlobalPosition[]= { 0, 10, 0, 1.0f };				 // Light Position ( NEW )
//                glLightfv( GL_LIGHT0, GL_AMBIENT, LightAmbientGlobal );
//                glEnable(GL_LIGHT0);
//                night = 1;
//                for( int i = 0; i < n_lights; i++ )
//                    glEnable(GL_LIGHT0+i+2);
//            }
//            else
//            {
//                float mcolor[] = { 0.6f, 0.6f, 0.6f, 1.0f };
//                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
//                night = 0;
//                glDisable(GL_LIGHT0);
//                GLfloat LightAmbientGlobal[] = { 0.9f, 0.9f, 0.9f, 1.0f };
//                GLfloat LightAmbientGlobalPosition[]= { 0, 10, 0, 1.0f };				 // Light Position ( NEW )
//                glLightfv( GL_LIGHT0, GL_AMBIENT, LightAmbientGlobal );
//                glEnable(GL_LIGHT0);
//                for( int i = 0; i < n_lights; i++ )
//                    glDisable(GL_LIGHT0+i+2);
//            }
//        }
//        break;
//    case Qt::Key_I:         // night mode on/off
//        GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				 // Diffuse Light Values ( NEW )
//        //GLfloat LightPosition[]= { x_light, y_light, z_light, 1.0f };				 // Light Position ( NEW )
//        GLfloat LightPosition[]= { xpos, ytrans, zpos, 1.0f };				 // Light Position ( NEW )
//        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
//        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
//        glEnable(GL_LIGHT1);
//        break;

//    /* GRAPHICS CONTROL */
//    case Qt::Key_B:         // translucency on/off (przenikanie)
//        break;

    /* DATA CONTROL */
    case Qt::Key_F5:        // reload map from files
        if(!World::instance()->initializeMap("data/map_zewnatrz.txt"))
            exitRequested();
        break;

    case Qt::Key_F1:        // fullscreen on/off
        m_window.toggleFullScreen();
        break;

//    /* SOUND */
//    case Qt::Key_X:
//        break;

//    /* UNKNOWN - TODO! */
//    case Qt::Key_V:
//        fp=TRUE;
//        filter+=1;
//        if (filter>2)
//        {
//            filter=0;
//        }
//        break;

    /* GLOBAL CONTROL */
    case Qt::Key_Escape:
        exitRequested();
    }
}

void MainSystem::mouseMoved(int x, int y)
{
    static int x_old = 0, y_old = 0;

    int xm = x_old - x;
    int ym = y_old - y;

    x_old = x;
    y_old = y;

    float sensitivity = 0.01f;

    World::instance()->getPlayer()->angle -= xm * sensitivity;
    World::instance()->getPlayer()->lx = sin(World::instance()->getPlayer()->angle);
    World::instance()->getPlayer()->lz = -cos(World::instance()->getPlayer()->angle);


    World::instance()->getPlayer()->angleY -= ym * sensitivity;
    World::instance()->getPlayer()->ly = sin(World::instance()->getPlayer()->angleY);
}
#endif
