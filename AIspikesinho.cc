#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME spikesinho


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
    typedef vector< vector<bool> > Visitado; //Matriz de pos visitadas
    typedef vector< vector<Pos> > Mapa; //Matriz de pos para marcar el camino
    const vector<Dir> dirs = {Up,Down,Left,Right};

    

    bool can_move_day(Pos nextp, int id){
      Cell c = cell(nextp);
        if(citizen(id).type == Warrior){
          if(c.type == Building or c.id != -1 or (c.b_owner != me() and c.b_owner != -1)) return false;
          return true;
        }
        else {
          if(c.type == Building or c.id != -1 or (c.b_owner != me() and c.b_owner != -1)) return false;
          return true;
        }
    }

    bool can_move_night(Pos nextp, int id){
      Cell c = cell(nextp);
      if(citizen(id).type == Warrior){
        if(c.type == Building or citizen(c.id).player == me()) return false;
        return true;
      }
      else{
        if(c.type == Building or citizen(c.id).player == me() or citizen(c.id).type == Warrior ) return false;
        return true;
      }
    }

    void make_move( Mapa& map, int id, Pos p_bw, Pos p_fin){
      bool found =  false;
      while(not found){
        if(map[p_fin.i][p_fin.j] == p_bw) found = true;
        else p_fin = map[p_fin.i][p_fin.j];
      }
      
      if(p_fin.i < p_bw.i) {
        move(id,Up);
        return;
      }
      if(p_fin.i > p_bw.i) { move(id,Down); return;}
      if(p_fin.j < p_bw.j) { move(id,Left); return; }
        if(p_fin.j > p_bw.j) {move(id,Right); return; }

    
    }

    void make_move_op(const Mapa& map, int id, Pos p_bw, Pos p_fin, bool& moved ){
      moved = true;
      bool found =  false;
      while(not found){
        if(map[p_fin.i][p_fin.j] == p_bw) found = true;
        else p_fin = map[p_fin.i][p_fin.j];
      }
      
      
      if(p_fin.i < p_bw.i) {
        if(pos_ok(pos_opi(Up,p_fin)) and cell(p_fin).type == Street) move(id,Down);
        else move_pari(p_bw,id);
        return;
      }
      if(p_fin.i > p_bw.i) { if(pos_ok(pos_opi(Down,p_fin)) and cell(p_fin).type == Street) move(id,Up); else move_pari(p_bw,id); return;}
      if(p_fin.j < p_bw.j) { if(pos_ok(pos_opj(Left,p_fin)) and cell(p_fin).type == Street) move(id,Right); else move_parj(p_bw,id); return; }
      if(p_fin.j > p_bw.j) {if(pos_ok(pos_opi(Right,p_fin)) and cell(p_fin).type == Street) move(id,Left); else move_pari(p_bw,id); return; }

    

    }

    
  void move_pari(Pos poss, int id){
    int r = random(1,2);
    if(r == 1 and pos_ok(poss) and cell(poss).type == Street ) move(id,Right);
    else if(r == 2  and pos_ok(poss) and cell(poss).type == Street) move(id,Left);
    return;

  }
  void move_parj(Pos poss, int id){
    int r = random(1,2);
    if(r == 1 and pos_ok(poss) and cell(poss).type == Street ) move(id,Up);
    else if(r == 2  and pos_ok(poss) and cell(poss).type == Street) move(id,Down);
    return;
  }

  Dir op_dir(Dir& direc){
    if(direc == Up) return Down;
    else if(direc == Down) return Up;
    else if(direc == Right) return Left;
    return Right;
  }

  Pos pos_opi(Dir direc, Pos& poss){
      if(direc == Up) poss.i += 1;
      else poss.i -= 1;
      return poss;
  }

  Pos pos_opj(Dir direc, Pos& poss){
      if(direc == Right) poss.j -= 1;
      else poss.j +=1;
      return poss;
  }
   

    void move_warriors(int id){
      
        int stop = 0;
        bool moved = false;
        int f = board_rows(); // filas mapa
        int c = board_cols(); // columnas mapa
        Visitado vis (f,vector<bool>(c,false)) ;// inicialmente ninguna pos visitada
        Mapa map(f,vector<Pos>(c)); //inicialmente vacía
        queue<Pos>q;
        Pos p = citizen(id).pos;
        vis[p.i][p.j] = true; // pos ini ya visitada 
        q.push(p);
        
        while(not q.empty()){
          Pos pos_w = q.front(); //posicion guerrero
          q.pop();
          ++stop;
          if(stop == 40){
            Dir random_dir = dirs[random(0,3)];
	    if (pos_ok(p+random_dir) and (cell(p+random_dir).id == -1 or citizen(cell(p+random_dir).id).type == Builder)) { // Move there if no citizen or builder	     
	      move(id,random_dir);
          return;
          }
          }
            for(Dir d : dirs){
              
              Pos next_pos = pos_w + d;
              
                if( is_day() and pos_ok(next_pos) and can_move_day(next_pos,id) and not vis[next_pos.i][next_pos.j]){ 
                  q.push(next_pos);
                  vis[next_pos.i][next_pos.j] = true;
                  map[next_pos.i][next_pos.j] = pos_w;
                  
                  if((cell(next_pos).weapon == Bazooka and citizen(id).weapon != Bazooka  ) or (cell(next_pos).weapon == Gun and citizen(id).weapon == Hammer) ){ make_move(map,id,pos_w,next_pos); return;}

                  

                   if(citizen(id).life <= life_lost_in_attack()){

                     if(cell(next_pos).bonus == Food){ make_move(map,id,pos_w,next_pos); return; }
                   }
                   if(cell(next_pos).bonus == Money){ make_move(map,id,pos_w,next_pos); return; }
                  
                  
                
                }

              
               //si es de noche
                if(is_night() and pos_ok(next_pos) and can_move_night(next_pos,id) and not vis[next_pos.i][next_pos.j]){
                  q.push(next_pos);
                  
                  vis[next_pos.i][next_pos.j] = true;
                  map[next_pos.i][next_pos.j] = pos_w;
                  if(citizen(id).life > life_lost_in_attack()){
                     if(cell(next_pos).id != -1 and citizen(cell(next_pos).id).player != me() and ((citizen(id).weapon > citizen(cell(next_pos).id).weapon and citizen(cell(next_pos).id).type == Warrior) or (citizen(cell(next_pos).id).type == Builder)   )) {make_move(map,id,pos_w,next_pos); return; }
                  }
                
                   //if(cell(next_pos).weapon != NoWeapon) {make_move(map,id,pos_w,next_pos); return;}
                  if( citizen(id).life <= life_lost_in_attack()){
                    if( cell(next_pos).id != -1 and citizen(cell(next_pos).id).player != me() and citizen(cell(next_pos).id).type == Builder) {
                      make_move(map,id,pos_w,next_pos);
                      return;
                    }
                    else if( cell(next_pos).id != -1 and citizen(cell(next_pos).id).player != me() and citizen(cell(next_pos).id).weapon > citizen(id).weapon) {
                      make_move_op(map,id,pos_w,next_pos,moved);
                      if(moved)return;
                      
                    }
                    else if(cell(next_pos).id != -1 and citizen(cell(next_pos).id).player != me() and citizen(cell(next_pos).id).weapon < citizen(id).weapon){make_move(map,id,pos_w,next_pos);}
                    

                      
                    }
                  }
                
              }
              
          }
          
            
        }
      
      

    

    void move_builders(int id){
      
        int stop  = 0;
        int fs = board_rows(); // filas mapa
        int c = board_cols(); // columnas mapa
        bool moved = false;
        Visitado vis (fs,vector<bool>(c,false)) ;// inicialmente ninguna pos visitada
        Mapa map(fs,vector<Pos>(c)); //inicialmente vacía
        queue<Pos>q;
        Pos p = citizen(id).pos;
        vis[p.i][p.j] = true; // pos ini ya visitada 
        q.push(p);
        
        while(not q.empty()){ // me falta implementar el booleano para acabar el bfs o break
          Pos pos_b = q.front(); //posicion guerrero
          q.pop();
          ++stop;
          /*if(stop == 120){
            Dir random_dir = dirs[random(0,3)];
	    if (pos_ok(p+random_dir) and (cell(p+random_dir).id == -1 or citizen(cell(p+random_dir).id).type == Builder)) { // Move there if no citizen or builder	     
	      move(id,random_dir);
          return;
          }
          }*/
            for(Dir d : dirs){
              
              Pos next_pos = pos_b + d;
              
                if(is_day() and pos_ok(next_pos) and can_move_day(next_pos,id) and not vis[next_pos.i][next_pos.j]){  
                  q.push(next_pos);
                  
                  vis[next_pos.i][next_pos.j] = true;
                  map[next_pos.i][next_pos.j] = pos_b;
                  if(pos_ok(next_pos) and cell(next_pos).b_owner == -1 and  cell(next_pos).is_empty() and int(barricades(me()).size()) < max_num_barricades()){ build(id,d); return; }
                  if(citizen(id).life <= life_lost_in_attack()){

                     if(cell(next_pos).bonus == Food){ make_move(map,id,pos_b,next_pos); return; }
                   }
                  if(cell(next_pos).bonus  == Money ){ make_move(map,id,pos_b,next_pos); return; }
                  

                }
              
              
                if(is_night() and pos_ok(next_pos) and can_move_night(next_pos,id) and not vis[next_pos.i][next_pos.j]){
                  
                  q.push(next_pos);
                  vis[next_pos.i][next_pos.j] = true;
                  map[next_pos.i][next_pos.j] = pos_b;
                  
                  if(cell(pos_b).b_owner == me()) {  return; }

                  if(cell(next_pos).b_owner == me()) { make_move(map,id,pos_b,next_pos); return; }
                  //if(citizen(id).life <= life_lost_in_attack() or citizen(cell(next_pos).id).type == Warrior){
                  //if(cell(next_pos).id != -1 and citizen(cell(next_pos).id).player != me() and citizen(cell(next_pos).id).type == Warrior  ) {make_move_op(map,id,pos_b,next_pos,moved); if(moved) return;  }
                if (cell(next_pos).id != -1 and cell(pos_b).b_owner == me() and weapon_strength_demolish(citizen(cell(next_pos).id).weapon) > cell(pos_b).resistance) { make_move_op(map,id,pos_b,next_pos,moved); if(moved) return;  }

                  if(cell(next_pos).bonus == Money) { make_move(map,id,pos_b,next_pos);  return; }
                  


                }
              
              
              }
            
            }
        
      }
    
    
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {

     vector<int> w = warriors(me()); // info de los guerreros que tengo
      for(int id : w) move_warriors(id);

    vector<int> b = builders(me()); // info de los obreros que tengo
    for(int id : b)move_builders(id);
  


  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
