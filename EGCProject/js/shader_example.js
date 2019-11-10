
var container;

var camera, scene, renderer;

var uniforms;

var numFilesLeft = 2;

function runMoreIfDone() {
    --numFilesLeft;
    if (numFilesLeft === 0) {
      init();
      animate();
    }
 }
 
 //******* DO NOT WORK
 var loader = new THREE.FileLoader();
 
 loader.load('js\\shader.vert',function ( data ) {vShader =  data; runMoreIfDone(); },);
 loader.load('js\\fragment.frag',function ( data ) {fShader =  data; runMoreIfDone(); },);



function init() {


    camera = new THREE.OrthographicCamera( - 1, 1, 1, - 1, 0, 1 );

    scene = new THREE.Scene();

    var geometry = new THREE.PlaneBufferGeometry( 2, 2 );

    uniforms = {
        "time": { value: 1.0 }
    };
    var numFilesLeft = 2;
  

    var material = new THREE.ShaderMaterial( {

        uniforms: uniforms,
        vertexShader: fShader,
        fragmentShader: vShader

    } );

    var mesh = new THREE.Mesh( geometry, material );
    scene.add( mesh );

    renderer = new THREE.WebGLRenderer();
    renderer.setPixelRatio( window.devicePixelRatio );
    document.body.appendChild( renderer.domElement );

    onWindowResize();

    window.addEventListener( 'resize', onWindowResize, false );

}

function onWindowResize() {

    renderer.setSize( window.innerWidth, window.innerHeight );

}

//

function animate( timestamp ) {

    requestAnimationFrame( animate );

    uniforms[ "time" ].value = timestamp / 1000;

    renderer.render( scene, camera );

}

