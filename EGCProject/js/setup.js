//cubes, fft related setup
var cubeNum, cubeSide = 5, padding;
var cubes, wireframe;
var fftSize;
var modeChanged = false;

//colors setup
var floorColor = 0xBBDEF0, skyColor = 0xBBDEF0, cubesColor = 0xF08700;
document.getElementById("menu").style.backgroundColor = "#BBDEF0";

// play pause setup
var audio, analyser, mediaElement;
document.getElementById( 'play' ).addEventListener( 'click', clickedPlay);
document.getElementById( 'pause' ).addEventListener( 'click', clickedPause);


function clickedPlay(){
    updateByMode();
    document.getElementById("startupInfo").style.display = "none";
    audio = new THREE.Audio( new THREE.AudioListener() );
    if (typeof mediaElement !== 'undefined') 
        mediaElement.pause()
    mediaElement = new Audio( document.getElementById("selector").value );
    mediaElement.loop = true;
    mediaElement.play();
    audio.setMediaElementSource( mediaElement );
    analyser = new THREE.AudioAnalyser( audio, fftSize*2 );
    document.getElementById("selector").value
    console.log("play");
}

function clickedPause(){
    console.log("pause");
    mediaElement.pause();
}

document.getElementById("mode").onchange = function(){
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

            break;    
    }
}

