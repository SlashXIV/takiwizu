Module.onRuntimeInitialized = () => {
    start();
  };
    

  function processGame(g, canvas, ctx, cell_width, cell_height) {

    
    var empty_color = "#FFFFFF";
    var white = "#BDECB6";
    var black = "#000000";
  
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);

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
        if (is_immutable){
            // draw a red cross on the cell
            ctx.strokeStyle = "red";
            ctx.lineWidth = 2;
            ctx.beginPath();
            ctx.moveTo(col * cell_width, row * cell_height);
            ctx.lineTo((col + 1) * cell_width, (row + 1) * cell_height);
            ctx.moveTo((col + 1) * cell_width, row * cell_height);
            ctx.lineTo(col * cell_width, (row + 1) * cell_height);
            ctx.stroke();
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

            case "undo":
                Module._undo(g);
                break;
            
            case "redo":
                Module._redo(g);
                break;

            case "solve":
                Module._solve(g);
                break;
            
            

        }



       


        updateGameState(game_state,game_end,game_in_process);
        
    }   
        
    );
    
    
    function updateGameState(game_state,game_end,game_in_process) {
        
        if (Module._is_over(g)) { // add a title in the page if the game is over
            game_end.innerHTML = "Jeu terminé !";
            game_state.innerHTML = "";
            canvas.removeEventListener('click', null);
            Module._delete(g);
            

        } else {
            game_state.innerHTML = "";
            game_in_process.innerHTML = "Le jeu est en cours";
            
        }


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
          if (is_immutable){
              // draw a red cross on the cell
              ctx.strokeStyle = "red";
              ctx.lineWidth = 2;
              ctx.beginPath();
              ctx.moveTo(col * cell_width, row * cell_height);
              ctx.lineTo((col + 1) * cell_width, (row + 1) * cell_height);
              ctx.moveTo((col + 1) * cell_width, row * cell_height);
              ctx.lineTo(col * cell_width, (row + 1) * cell_height);
              ctx.stroke();
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
  
    }
  }


function start() {
    console.log("call start routine");
    var g = Module._new_random(6,6,false,false);
    var canvas = document.getElementById('result');
    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var height = canvas.height;
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    var cell_width = width / nb_cols;
    var cell_height = height / nb_rows;
    var game_state = document.getElementById("game_state");
    var game_end = document.getElementById("game_end");
    var game_in_process = document.getElementById("game_in_process");
    game_state.innerHTML = "Le jeu vient d'apparaître, tentez votre premier coup!";
  
    
  
    processGame(g, canvas, ctx, cell_width, cell_height, game_state);   

  }
