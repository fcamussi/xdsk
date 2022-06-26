# xdsk

Extractor para MSX de ficheros de imágenes de disquetes .dsk en formato FAT12.

## Captura

<img src="https://user-images.githubusercontent.com/75378876/175793620-291bc5bb-6e9a-4d79-b270-2190b86fb467.png" alt="screenshot1" width="600">

## Requisitos

* Compilador SDCC versión 3.4.0
* MSX-DOS (para la ejecución)

## Compilación

Descargar SDCC versión 3.4.0 y descomprimir. Luego, supondiendo que $SDCC_DIR contiene la ruta donde se descomprimió SDCC, tipear:

```
export PATH=$PATH:$SDCC_DIR/bin:$SDCC_DIR/share
cd backend-msxdos
make
cd ../xdsk
make
```

## Ejecución

```
xdsk fichero.dsk
```

## Versión compilada

Se puede descargar una versión compilada desde https://github.com/fcamussi/xdsk/tree/main/build

## Por hacer

* Soporte para subdirectorios
