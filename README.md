# Ricochet Controller

Manages data received by the Kinect and the cubes for drawing the projection.
Contains [submodules](https://git-scm.com/book/fr/v2/Utilitaires-Git-Sous-modules)
[ricochet_server](https://github.com/ricochetGobz/ricochet_server) and
[ricochet_render](https://github.com/ricochetGobz/ricochet_render).

## How to install

- Go to `addons/` into your OF folder and install [ofxControlPanel](https://github.com/ofTheo/ofxControlPanel) with a `git clone git@github.com:ofTheo/ofxControlPanel.git`.

- `git clone --recursive git@github.com:ricochetGobz/ricochet_controller.git`

or

- `git clone git@github.com:ricochetGobz/ricochet_controller.git`
- `git submodule init`
- `git submodule update`


## Exemples

- **Mapping :**
  - [Graffiti Research Lab L.A.S.E.R Tag](https://www.youtube.com/watch?v=DKbtTPYZEig)
  - [CubeMapped](https://vimeo.com/146400251)

- **OpenFrameworks app :**
  - **TODO**  


## Important Components

- **Application OpenFramework :**  

    - Analyse les données vidéo captés par la caméra.
    - Détecte la position des cubes dans l'espace.
    - Gère la timeline entre les différents cubes en fonction de leur positions et l'ordre de lecture.
    - Traduit en vibration la composition musicale qu'il transmet au bracelet.
    - Génère les différents visuels liés aux différents évènements captés par les cubes.

- **Kinect :** Captation vidéo des cubes au niveau de la position du vidéo projecteur.

- **Vidéo projecteur :** Mapping et projection des effets visuels et graphiques sur les cubes et le support de composition. Projection d'informations pour guider les utilisateurs.


## Links


## UML


## Functionnalities
