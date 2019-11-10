//world setup
var scene, camera, render, controls;

// Cubes, fft related setup
var cubeNum, cubeSide = 5, padding;
var cubes, wireframe;
var fftSize;
var modeChanged = false;

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

function clearCubes(){
    if(typeof cubes != 'undefined')
        for(var i = 0; i < cubes.length; i++)
            for (var j = 0; j < cubes[i].length; j++){
                scene.remove(scene.getObjectByName("cube" + i + j));  
                scene.remove(scene.getObjectByName("frame" + i + j));  
            }
}

function clickedPlay(){
    
    clearCubes();

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
    camera = new THREE.PerspectiveCamera( 90, window.innerWidth/window.innerHeight, 0.1, 1000 );

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

function initCubes(){

    //clear peviously existing cubes
    clearCubes();
    
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
    clearCubes();
    updateByMode();
}

function updateByMode(){

    var mode = document.getElementById("mode").value;
    modeChanged = true;
    switch (mode){
        case "1":
            padding = 3;
            cubeNum = 27, cubeSide = 5;
            cubes = Array(cubeNum).fill().map(()=>Array(cubeNum).fill());
            wireframe = Array(cubeNum).fill().map(()=>Array(cubeNum).fill());
            fftSize = (cubeNum + padding*2 - 1)*2;
        case "2":

            break;
        case "3":

            break;
        case "4":
            camera = new THREE.OrthographicCamera( - 1, 1, 1, - 1, 0, 1 );
            scene = new THREE.Scene();
            var geometry = new THREE.PlaneBufferGeometry( 2, 2 );
            uniforms = {
                "time": { value: 1.0 }
            };
            var material = new THREE.ShaderMaterial( {
                uniforms: uniforms,
                vertexShader: document.getElementById( 'vertexShader' ).textContent,
                fragmentShader: document.getElementById( 'fragmentShader' ).textContent
            } );
            var mesh = new THREE.Mesh( geometry, material );
            scene.add( mesh );

            break;    
    }
}


var animate = function () {
    if (modeChanged){
        initCubes();
        modeChanged = false;
    }
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

                break;
            case "3":

                break;
            case "4":
                console.log (timestamp/1000);
				requestAnimationFrame( animate );
				uniforms[ "time" ].value = (timestamp/1000);//*Math.abs(Math.sin(timestamp/1000));
                break;    
        }
        renderer.render( scene, camera );
    }
};

animate();

