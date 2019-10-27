var scene, camera, render, controls;

initWorld();
initCubes();

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

    var geo = new THREE.PlaneBufferGeometry(2000, 2000, 8, 8);
    var mat = new THREE.MeshBasicMaterial({ color: floorColor, side: THREE.DoubleSide });
    var plane = new THREE.Mesh(geo, mat);
    plane.position.y -= cubeSide;
    plane.rotateX( - Math.PI / 2);
    scene.add(plane);

}

function initCubes(){
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

var animate = function () {
    requestAnimationFrame( animate );
    analyser.getFrequencyData();
    var data = analyser.data;
    for (var i = 0 ; i < data.length; i++){
        cubes[0][i].scale.set( 1, Math.max(1, data[i]/2), 1);
        wireframe[0][i].scale.set( 1, Math.max(1, data[i]/2), 1);
    }
    cubes[0][0].rotation.x += 0.01;
    cubes[0][0].rotation.y += 0.01;
    wireframe[0][0].rotation.x += 0.01;
    wireframe[0][0].rotation.y += 0.01;

    renderer.render( scene, camera );
};

animate();

