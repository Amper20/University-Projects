//world setup
var scene, camera, render, controls;

// Cubes, fft related setup
var cubeNum, cubeSide = 5, padding;
var cubes, wireframe;
var fftSize = 64;
var modeChanged = false;

// Points mode
var SEPARATION = 55, AMOUNTX = 100, AMOUNTY = 128;
var particles, count = 0;
var mouseX = 0, mouseY = 0;
var windowHalfX = window.innerWidth / 2;
var windowHalfY = window.innerHeight / 2;


// Mushrums mode
var uniforms;

// play pause setup
var audio, analyser, mediaElement;

//colors setup
var floorColor = 0xBBDEF0, skyColor = 0xBBDEF0, cubesColor = 0xF08700;

document.getElementById("menu").style.backgroundColor = "#BBDEF0";
document.getElementById( 'play' ).addEventListener( 'click', clickedPlay);
document.getElementById( 'pause' ).addEventListener( 'click', clickedPause);

initWorld();
updateByMode();

function clearCubes(){
    if(typeof cubes != 'undefined')
        for(var i = 0; i < cubes.length; i++)
            for (var j = 0; j < cubes[i].length; j++){
                scene.remove(scene.getObjectByName("cube" + i + j));  
                scene.remove(scene.getObjectByName("frame" + i + j));  
            }
}

function clearPoints(){
    scene.remove(particles);
}

function clickedPlay(){
    
    updateByMode();
    
    document.getElementById("startupInfo").style.display = "none";
    
    // setup media element
    if (typeof mediaElement !== 'undefined') 
        mediaElement.pause()
    mediaElement = new Audio( document.getElementById("selector").value );
    mediaElement.loop = true;
    mediaElement.play();

    // audio stream for FFT
    audio = new THREE.Audio( new THREE.AudioListener() );
    audio.setMediaElementSource( mediaElement );
    
    // FFT
    analyser = new THREE.AudioAnalyser( audio, fftSize*2 );

    console.log("play");
}

function clickedPause(){

    mediaElement.pause();
    
    console.log("pause");
}


function initWorld(){
    // set up scene and camera
    scene = new THREE.Scene();
    camera = new THREE.PerspectiveCamera( 90, window.innerWidth/window.innerHeight, 0.1, 10000 );

    // set up renderer 
    renderer = new THREE.WebGLRenderer();
    renderer.setSize( window.innerWidth, window.innerHeight );
    renderer.setClearColor( skyColor, 1 );
    document.body.appendChild( renderer.domElement );

    //positionate camera
    controls = new THREE.OrbitControls( camera, renderer.domElement );
    camera.position.set( -133.7817033929154, 95.38754075877799, 154.8839474041323);
    camera.rotation.set(-0.5520030533709762, -0.6341343221091368, -0.34987573749653283);

    //controls.update() must be called after any manual changes to the camera's transform
    controls.update();
}

function initPoints(){
    var numParticles = AMOUNTX * AMOUNTY;

    var positions = new Float32Array( numParticles * 3 );
    var scales = new Float32Array( numParticles );

    var i = 0, j = 0;

    for ( var ix = 0; ix < AMOUNTX; ix ++ ) {

        for ( var iy = 0; iy < AMOUNTY; iy ++ ) {

            positions[ i ] = ix * SEPARATION - ( ( AMOUNTX * SEPARATION ) / 2 ); // x
            positions[ i + 1 ] = 0; // y
            positions[ i + 2 ] = iy * SEPARATION - ( ( AMOUNTY * SEPARATION ) / 2 ); // z

            scales[ j ] = 1;

            i += 3;
            j ++;

        }

    }

    var geometry = new THREE.BufferGeometry();
    geometry.setAttribute( 'position', new THREE.BufferAttribute( positions, 3 ) );
    geometry.setAttribute( 'scale', new THREE.BufferAttribute( scales, 1 ) );

    var material = new THREE.ShaderMaterial( {
        
        uniforms: {
            color: { value: new THREE.Color( 0x000000 ) },
        },
        vertexShader: document.getElementById( 'vertexshader' ).textContent,
        fragmentShader: document.getElementById( 'fragmentshader' ).textContent

    } );

    //

    particles = new THREE.Points( geometry, material );
    scene.add( particles );

    //
}

function initCubes(){

    //clear peviously existing cubes
    clearCubes();
    
    padding = 3;
    cubeNum = 27, cubeSide = 5;
    cubes = Array(cubeNum).fill().map(()=>Array(cubeNum).fill());
    wireframe = Array(cubeNum).fill().map(()=>Array(cubeNum).fill());
    fftSize = (cubeNum + padding*2 - 1)*2;

    var geometry = new THREE.BoxGeometry( cubeSide, cubeSide, cubeSide );
    var material = new THREE.MeshBasicMaterial( { color: cubesColor } );
    var lineMaterial = new THREE.LineBasicMaterial({ color: 0xBBDEF0});
            
    for(var i = 0 ; i < cubeNum; i++){
        for(var j = 0 ; j < cubeNum; j++){            

            cubes[i][j] = new THREE.Mesh( geometry, material);
            cubes[i][j].position.x = (i - cubeNum/2)*cubeSide;
            cubes[i][j].position.z = (j - cubeNum/2)*cubeSide;
            cubes[i][j].name = "cube" + i + j;
            scene.add(cubes[i][j]);

            wireframe[i][j] = new THREE.LineSegments(  new THREE.EdgesGeometry(cubes[i][j].geometry), lineMaterial);
            wireframe[i][j].position.x = (i - cubeNum/2)*cubeSide;
            wireframe[i][j].position.z = (j - cubeNum/2)*cubeSide;
            wireframe[i][j].name = "frame" + i + j;
            scene.add( wireframe[i][j] );
        }
    }   

}

document.getElementById("mode").onchange = function(){
    updateByMode();
}

function updateByMode(){

    clearCubes();
    clearPoints();
    var mode = document.getElementById("mode").value;
    switch (mode){
        case "1":
            initCubes();
        case "2":
            initPoints();   
            break;
        case "3":
            break;
        case "4":
            break;    
    }
}


var animate = function () {
    
    requestAnimationFrame( animate );
    if(typeof analyser != 'undefined'){
        analyser.getFrequencyData();
        var data = analyser.data;
        var mode = document.getElementById("mode").value;
        switch (mode){
            case "1":
                for (var i = 0 ; i < cubes.length; i++){
                    for (var j = 0 ; j < cubes.length; j++){   
                        cubes[i][j].scale.set( 1, Math.max(0.001, data[i+j]/4), 1);
                        wireframe[i][j].scale.set( 1, Math.max(0.001, data[i+j]/4 ), 1);
                    }
                }          
                break;
            case "2":
                var positions = particles.geometry.attributes.position.array;
				var scales = particles.geometry.attributes.scale.array;

				var i = 0, j = 0;

				for ( var ix = 0; ix < AMOUNTX; ix ++ ) {

					for ( var iy = 0; iy < AMOUNTY; iy ++ ) {

						positions[ i + 1 ] = ( Math.sin( ( ix + count ) * 0.3 ) * 50 ) +
										( Math.sin( ( iy + count ) * 0.5 ) * 50 ) + data[ix] * 3;

						scales[ j ] = ( Math.sin( ( ix + count ) * 0.3 ) + 1 ) * 10 +
										( Math.sin( ( iy + count ) * 0.5 ) + 1 ) * 10 + data[ix]/5;

						i += 3;
						j ++;

					}

				}

				particles.geometry.attributes.position.needsUpdate = true;
				particles.geometry.attributes.scale.needsUpdate = true;
				count += 0.1;
                break;
            case "3":

                break;
            case "4":
                break;    
        }
        renderer.render( scene, camera );
    }
};

animate();

