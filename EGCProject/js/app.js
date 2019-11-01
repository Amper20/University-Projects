var scene, camera, render, controls;

initWorld();

function initWorld(){
    //set up scene and camera
    scene = new THREE.Scene();
    camera = new THREE.PerspectiveCamera( 90, window.innerWidth/window.innerHeight, 0.1, 1000 );

    renderer = new THREE.WebGLRenderer();
    renderer.setSize( window.innerWidth, window.innerHeight );
    renderer.setClearColor( skyColor, 1 );
    document.body.appendChild( renderer.domElement );

    //positionate camera
    controls = new THREE.OrbitControls( camera, renderer.domElement );
    camera.position.set( -9.277557506533654, 57.32464742248635, -12.976140669356077);
    camera.rotation.set(-2.3797778235060614, -0.515317861964013, -3.0078375034025826);
    //controls.update() must be called after any manual changes to the camera's transform
    controls.update();
}

function initCubes(){
    clearCubes();
    var geometry = new THREE.BoxGeometry( cubeSide, cubeSide, cubeSide );
    var material = new THREE.MeshBasicMaterial( { color: cubesColor } );
    for(var i = 0 ; i < cubeNum; i++){
        cubes[i] = new Array(cubeNum);
        wireframe[i] = new Array(cubeNum);
        for(var j = 0 ; j < cubeNum; j++){            
            cubes[i][j] = new THREE.Mesh( geometry, material);
            cubes[i][j].position.x = (i - cubeNum/2)*cubeSide;
            cubes[i][j].position.z = (j - cubeNum/2)*cubeSide;
            scene.add(cubes[i][j]);
            var lineMaterial = new THREE.LineBasicMaterial({ color: 0xBBDEF0});
            wireframe[i][j] = new THREE.LineSegments(  new THREE.EdgesGeometry(cubes[i][j].geometry), lineMaterial);
            wireframe[i][j].position.x = (i - cubeNum/2)*cubeSide;
            wireframe[i][j].position.z = (j - cubeNum/2)*cubeSide;
            scene.add( wireframe[i][j] );
        }
    }   

}

function clearCubes(){
    if(typeof cubes != 'undefined'){
        for(var i = 0; i < cubes.length; i++){
            for (var j = 0; j < cubes[i].length; j++){
                scene.remove( scene.getObjectByName(""));  
            }
        }
    }
}

var animate = function () {
    if (modeChanged){
        clearCubes();
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
                        cubes[i][j].scale.set( 1, Math.max(1, data[i+j]/4), 1);
                        wireframe[i][j].scale.set( 1, Math.max(1, data[i+j]/4 ), 1);
                    }
                }          
                break;
            case "2":

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
