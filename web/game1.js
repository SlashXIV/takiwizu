Module.onRuntimeInitialized = () => { start(); };

function start() {
    var g = Module._new_random(6, 6, false, false);
    processGame(g);
}

function gamePrint(g, nb_rows, nb_cols, ctx, cell_width, cell_height) {
    var empty_color = "#FFFFFF";
    var white = "#BDECB6";
    var black = "#000000";

    // draw the cells
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            var square_number = Module._get_number(g, row, col);
            var is_immutable = Module._is_immutable(g, row, col);
            switch (square_number) {
                case -1:
                    ctx.fillStyle = empty_color;
                    break;
                case 0:
                    ctx.fillStyle = white;
                    break;
                case 1:
                    ctx.fillStyle = black;
                    break;
            }
            ctx.fillRect(col * cell_width, row * cell_height, cell_width, cell_height);
            if (is_immutable) {

                ctx.fillStyle = "#8dc73f";
                ctx.beginPath();
                ctx.arc((col + 0.5) * cell_width, (row + 0.5) * cell_height, 0.4 * cell_width, 0, 2 * Math.PI);
                ctx.fill();

                ctx.fillStyle = "#6a9a3c";
                ctx.beginPath();
                ctx.arc((col + 0.5) * cell_width, (row + 0.5) * cell_height, 0.3 * cell_width, 0, 2 * Math.PI);
                ctx.fill();

                ctx.fillStyle = "#ffffff";
                ctx.beginPath();
                ctx.arc((col + 0.4) * cell_width, (row + 0.4) * cell_height, 0.04 * cell_width, 0, 2 * Math.PI);
                ctx.fill();

                ctx.fillStyle = "#ffffff";
                ctx.beginPath();
                ctx.arc((col + 0.6) * cell_width, (row + 0.4) * cell_height, 0.04 * cell_width, 0, 2 * Math.PI);
                ctx.fill();

                ctx.fillStyle = "#3d3d3b";
                ctx.beginPath();
                ctx.moveTo((col + 0.44) * cell_width, (row + 0.48) * cell_height);
                ctx.lineTo((col + 0.56) * cell_width, (row + 0.48) * cell_height);
                ctx.lineTo((col + 0.5) * cell_width, (row + 0.70) * cell_height);
                ctx.closePath();
                ctx.fill();
            }
        }
    }


    // apply border to each rect in the canvas 
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            ctx.strokeStyle = "grey";
            ctx.lineWidth = 2;
            ctx.strokeRect(col * cell_width, row * cell_height, cell_width, cell_height);
        }
    }

    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            if (Module._has_error(g, row, col)) {
                ctx.fillStyle = "rgba(255, 0, 0, 0.2)";
                ctx.fillRect(col * cell_width, row * cell_height, cell_width, cell_height);


            }
        }
    }
}

function processGame(g) {

    var canvas = document.getElementById('result');
    var ctx = canvas.getContext('2d');

    var width = canvas.width;
    var height = canvas.height;

    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    var cell_width = width / nb_cols;
    var cell_height = height / nb_rows;

    gamePrint(g, nb_cols, nb_rows, ctx, cell_width, cell_height);

    var undo_button = document.getElementById("undo");
    var redo_button = document.getElementById("redo");
    var solve_button = document.getElementById("solve");
    var restart_button = document.getElementById("restart");

    undo_button.addEventListener('click', function(e) {
        console.log("undo");
        Module._undo(g);
        gamePrint(g, nb_cols, nb_rows, ctx, cell_width, cell_height);
    });

    redo_button.addEventListener('click', function(e) {
        console.log("redo");
        Module._redo(g);
        gamePrint(g, nb_cols, nb_rows, ctx, cell_width, cell_height);
    });

    solve_button.addEventListener('click', function(e) {
        console.log("solve");
        Module._solve(g);
        gamePrint(g, nb_cols, nb_rows, ctx, cell_width, cell_height);
    

        if (Module._is_over(g)) {
            var popup = document.getElementById("popup");
            var closeButton = document.getElementById("close-button");
            popup.style.display = "block";
            closeButton.addEventListener("click", function() {
              popup.style.display = "none";
            });
          }
        

    });

    restart_button.addEventListener('click', function(e) {
        console.log("restart");
        Module._restart(g);
        gamePrint(g, nb_cols, nb_rows, ctx, cell_width, cell_height);
    });

    // event listener puts a black cell on the canvas where the user mouse is 
    canvas.addEventListener('click', function(e) {
        console.log("click");
        var rect = canvas.getBoundingClientRect();
        var x = e.clientX - rect.left;
        var y = e.clientY - rect.top;
        var row = Math.floor(y / cell_height);
        var col = Math.floor(x / cell_width);
        console.log("existing case:", Module._get_number(g, row, col));
        console.log("row: " + row + " col: " + col);
        console.log("game dimensions:", Module._nb_cols(g) + "x" + Module._nb_rows(g));
        var form = document.getElementById("square_selector");

        if (Module._is_immutable(g, row, col)) {
            console.log("case immutable");
            return;
        }


        for (var i = 0; i < form.elements.length; i++) {
            if (form.elements[i].checked) {
                var square_to_place = form.elements[i].value;
                console.log("square to place: " + square_to_place);
            }
        }


        switch (square_to_place) {
            case "black":
                if (Module._get_number(g, row, col) != 1) {
                    Module._play_move(g, row, col, 2);
                } else {
                    console.log("case already black")
                }
                break;

            case "white":
                if (Module._get_number(g, row, col) != 0) {
                    Module._play_move(g, row, col, 1);
                } else {
                    console.log("case already white")
                }
                break;

            case "empty":
                if (!Module._is_empty(g, row, col)) {
                    Module._play_move(g, row, col, 0);
                } else {
                    console.log("case already empty")

                }
                break;
        }
        gamePrint(g, nb_cols, nb_rows, ctx, cell_width, cell_height);
    });


}