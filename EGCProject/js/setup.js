//cubes related setup
var cubeNum = 64, cubeSide = 5;
var cubes = Array(cubeNum).fill().map(()=>Array(cubeNum).fill());
var wireframe = Array(cubeNum).fill().map(()=>Array(cubeNum).fill());

//colors setup
var floorColor = 0xBBDEF0, skyColor = 0xBBDEF0, cubesColor = 0xF08700;
document.getElementById("menu").style.backgroundColor = "#BBDEF0";

//fft 
var fftSize = cubeNum;

// play pause setup
var audio, analyser, mediaElement;
document.getElementById( 'play' ).addEventListener( 'click', clickedPlay);
document.getElementById( 'pause' ).addEventListener( 'click', clickedPause);

function clickedPlay(){
    audio = new THREE.Audio( new THREE.AudioListener() );
    if (typeof mediaElement !== 'undefined') 
        mediaElement.pause()
    mediaElement = new Audio( document.getElementById("selector").value );
    mediaElement.loop = true;
    mediaElement.play();
    audio.setMediaElementSource( mediaElement );
    analyser = new THREE.AudioAnalyser( audio, fftSize );
    document.getElementById("selector").value
    console.log("play");
}

function clickedPause(){
    console.log("pause");
    mediaElement.pause();
}