{-# OPTIONS_GHC -Wall #-}
{-# LANGUAGE EmptyDataDecls, MultiParamTypeClasses,
             TypeSynonymInstances, FlexibleInstances,
             InstanceSigs #-}


module Bloxorz where

import ProblemState

import qualified Data.Array as A

{-
    Caracterele ce vor fi printate pentru fiecare tip de obiect din joc 
    Puteți înlocui aceste caractere cu orice, în afară de '\n'.
-}

-- hardTile :: Char
-- hardTile = '▒'

-- softTile :: Char
-- softTile = '='

-- block :: Char
-- block = '▓'

-- switch :: Char
-- switch = '±'

-- emptySpace :: Char
-- emptySpace = ' '

-- winningTile :: Char
-- winningTile = '*'

hardTile :: Char
hardTile = 'H'

softTile :: Char
softTile = 'S'

block :: Char
block = 'X'

switch :: Char
switch = '+'

emptySpace :: Char
emptySpace = ' '

winningTile :: Char
winningTile = '*'

{-
    Sinonim de tip de date pentru reprezetarea unei perechi (int, int)
    care va reține coordonatele de pe tabla jocului
-}

type Position = (Int, Int)

{-
    Direcțiile în care se poate mișcă blocul de pe tablă
-}

data Directions = North | South | West | East
    deriving (Show, Eq, Ord)

{-
    *** TODO ***

    Tip de date care va reprezenta plăcile care alcătuiesc harta și switch-urile
-}

data Cell = Cell {cellType::Char, pos::Position} | Switch {cellType::Char, pos::Position, state::Int, onClickList::[Position]} deriving (Eq)

instance Show Cell where
    show (Cell x _) = show x
    show (Switch x _ _ _) = show x

{-
    *** TODO ***

    Tip de date pentru reprezentarea nivelului curent
-}

type Mat = (A.Array Position Cell)
data Level = Level {mat :: Mat, blck :: [Position], dpt :: Int} 

{-
    *** Opțional *** 
  
    Dacă aveți nevoie de o funcționalitate particulară, 
    instantiati explicit clasele Eq și Ord pentru Level. 
    În cazul acesta, eliminați deriving (Eq, Ord) din Level. 
-}

instance Eq Level where
    (==) (Level matA blockPosA depthA) (Level matB blockPosB depthB) = (blockPosA == blockPosB) && ((A.elems matA) == (A.elems matB)) && depthB == depthA

instance Ord Level where
    compare (Level _ blockPosA _) (Level _ blockPosB _) 
        | blockPosA < blockPosB = LT
        | blockPosA == blockPosB = EQ
        | otherwise = GT

{-
    *** TODO ***

    Instantiati Level pe Show. 

    Atenție! String-ul returnat va fi urmat și precedat de un rând nou. 
    În cazul în care jocul este câștigat, la sfârșitul stringului se va mai
    concatena mesajul "Congrats! You won!\n". 
    În cazul în care jocul este pierdut, se va mai concatena "Game Over\n". 
-}

getCh::Cell->Char
getCh (Cell x _) = x
getCh (Switch x _ _ _) = x

dispCell::Level->Position->String
dispCell (Level arr blockPos _) (i,j)
    |j == (snd (snd (A.bounds arr)) + 1) && i <  (fst (snd (A.bounds arr)) + 1) = "\n" 
    |elem (i,j) blockPos = show block
    |otherwise = show (arr A.! (i,j))

gameState::Level -> Int
gameState lvl
        | (dpt lvl) == 0 = 0
        | (getWinCell lvl) == (head (blck lvl)) && (length (blck lvl)) == 1 = 1 --Win--
        | (compLists (getEmptyCells lvl) (blck lvl)) || (not (inBounds (snd (A.bounds (mat lvl)))  (blck lvl))) || ((compLists (getSoftCells lvl) (blck lvl)) && (length(blck lvl)) == 1)  = -1 --Lose-- --check if standing on soft tile-- 
        | otherwise = 0 --Continue--

instance Show Level where
    show (Level arr blockPos depth) =  filter (/= '\'') $ ("\n" ++ (concat [(dispCell (Level arr blockPos depth) (i, j)) | i <- [0..(fst(snd (A.bounds arr)))] , j <- [0..((snd (snd (A.bounds arr))) + 1)]]) ++ final )
                        where final = if (gameState (Level arr blockPos depth) == 1)
                                        then "Congrats! You won!\n"
                                        else if (gameState (Level arr blockPos depth) == -1)
                                                then "Game Over\n"
                                                else ""

                          

{-
    *** TODO ***

    Primește coordonatele colțului din dreapta jos a hârtii și poziția inițială a blocului.
    Întoarce un obiect de tip Level gol.
    Implicit, colțul din stânga sus este (0, 0).
-}

emptyLevel :: Position -> Position -> Level
emptyLevel size bpoz = (Level arr [bpoz] 0)
                where  arr = A.array ((0,0), (fst size, snd size)) --size--
                                    [((i,j), (Cell emptySpace (i,j))) | i <- [0..(fst size)] , j <- [0..(snd size)] ] --data--


{-
    *** TODO ***

    Adaugă o celulă de tip Tile în nivelul curent.
    Parametrul char descrie tipul de tile adăugat: 
        'H' pentru tile hard 
        'S' pentru tile soft 
        'W' pentru winning tile 
-}

addTile :: Char -> Position -> Level -> Level
addTile tileType position (Level arr blockPos depth) 
    |tileType == 'H' = (Level (arr A.// [ (position, Cell hardTile position) ]) blockPos depth)
    |tileType == 'S' = (Level (arr A.// [ (position, Cell softTile position) ]) blockPos depth)
    |tileType == 'W' = (Level (arr A.// [ (position, Cell winningTile position) ]) blockPos depth)
    |tileType == 'E' = (Level (arr A.// [ (position, Cell emptySpace position) ]) blockPos depth)
                                            
{-
    *** TODO ***

    Adaugă o celulă de tip Swtich în nivelul curent.
    Va primi poziția acestuia și o listă de Position
    ce vor desemna pozițiile în care vor apărea sau 
    dispărea Hard Cells în momentul activării/dezactivării
    switch-ului.
-}

addSwitch :: Position -> [Position] -> Level -> Level
addSwitch switchPos lst (Level arr blockPos depth) = (Level (arr A.// [ (switchPos, (Switch switch switchPos 0 lst )) ]) blockPos depth)

{-
    === MOVEMENT ===
-}

{-
    *** TODO ***

    Activate va verifica dacă mutarea blocului va activa o mecanică specifică. 
    În funcție de mecanica activată, vor avea loc modificări pe hartă. 
-}

updateTiles::[Position]->Char->Level->Level
updateTiles coordList tip level = foldr (\x y-> addTile tip x y) level coordList

changeSwitchState::Level->Cell->Level
changeSwitchState (Level arr blockPos depth) (Switch tip switchPos state coordList) = (Level (arr A.// [ (switchPos, (Switch tip switchPos ((state + 1) `mod` 2) coordList )) ]) blockPos depth)

activate :: Cell -> Level -> Level
activate (Switch tip position state coordList) level = if (state == 0)
                                                then (changeSwitchState (updateTiles coordList 'H' level) (Switch tip position state coordList))
                                                else (changeSwitchState (updateTiles coordList 'E' level) (Switch tip position state coordList))
activate (Cell _ _) level = level

{-
    *** TODO ***

    Mișcarea blocului în una din cele 4 direcții 
    Hint: Dacă jocul este deja câștigat sau pierdut, puteți lăsa nivelul neschimbat.
-}


compLists::(Eq list) => [list]->[list]->Bool
compLists listA listB = foldl (\acumulator x -> x `elem` listB || acumulator) False listA

getWinCell::Level->Position
getWinCell (Level arr _ _) =
    if((filter (\cell -> (cellType cell) == winningTile) (A.elems arr)) == [])
    then (0,0)
    else (pos  (head (filter (\cell -> (cellType cell) == winningTile) (A.elems arr))))

getEmptyCells::Level->[Position]
getEmptyCells (Level arr _ _) =
    map pos (filter (\cell -> (cellType cell) == emptySpace) (A.elems arr))

getSoftCells::Level->[Position]
getSoftCells (Level arr _ _) =
        map pos (filter (\cell -> (cellType cell) == softTile) (A.elems arr))

checkBound::Position->Position->Bool
checkBound  y x = ((fst y) >= 0) && ((snd y) >= 0) && ((fst y) <= (fst x)) && ((snd y) <= (snd x))  

inBounds::Position->[Position]->Bool    
inBounds  (n,m) blockPos 
    | (length blockPos) == 2 = (checkBound (head blockPos) (n,m)) && (checkBound (head (tail blockPos)) (n,m)) 
    | otherwise = (checkBound (head blockPos) (n,m))

mini::Int->Int->Int
mini x y
    | x < y = x
    | otherwise = y

maxi::Int->Int->Int
maxi x y
    | x > y = x
    | otherwise = y

getCellAt::Level->Position->Cell
getCellAt (Level arr _ _) (i,j) = (arr A.! (i,j))

move :: Directions -> Level -> Level
move North level =  
    if (gameState level) == 0
        then if(length (blck level) == 2)
                then if(y1 == y)
                        then (activate (getCellAt level ((mini x x1) - 1,y)) (Level (mat level) [((mini x x1) - 1,y)] ((dpt level) + 1)))
                        else (activate (getCellAt level (x1-1,y1)) (activate (getCellAt level (x-1,y)) (Level (mat level) [(x-1,y), (x1-1,y1)] ((dpt level) + 1)) ))
                else  (activate (getCellAt level (x-1,y)) (activate (getCellAt level (x-2,y)) (Level (mat level) [(x-1,y) , (x-2,y)] ((dpt level) + 1))))
        else level     

    where x = (fst$head$blck level) 
          y = (snd$head$blck level)   
          x1 = (fst$head$tail$blck level) 
          y1 = (snd$head$tail$blck level)

move South level = 
    if (gameState level) == 0
        then if(length (blck level) == 2)
                then if(y1 == y)
                        then (activate (getCellAt level ((maxi x x1) + 1,y)) (Level (mat level) [((maxi x x1) + 1,y)] ((dpt level) + 1)))
                        else (activate (getCellAt level (x1+1,y1)) (activate (getCellAt level (x-1,y)) (Level (mat level) [(x+1,y), (x1+1,y1)] ((dpt level) + 1)) ))
                else  (activate (getCellAt level (x+1,y)) (activate (getCellAt level (x+2,y)) (Level (mat level) [(x+1,y) , (x+2,y)] ((dpt level) + 1))))
        else level     

    where x = (fst$head$blck level) 
          y = (snd$head$blck level)   
          x1 = (fst$head$tail$blck level) 
          y1 = (snd$head$tail$blck level)

move West level=
    if (gameState level) == 0
        then if(length (blck level) == 2)
                then if(x1 == x)
                        then (activate (getCellAt level (x,(mini y y1)-1)) (Level (mat level) [(x,(mini y y1)-1)] ((dpt level) + 1)))
                        else (activate (getCellAt level (x1,y1-1)) (activate (getCellAt level (x,y-1)) (Level (mat level) [(x,y-1), (x1,y1-1)] ((dpt level) + 1)) ))
                else  (activate (getCellAt level (x,y-1)) (activate (getCellAt level (x,y-2)) (Level (mat level) [(x,y-1) , (x,y-2)] ((dpt level) + 1))))
        else level     

    where x = (fst$head$blck level) 
          y = (snd$head$blck level)   
          x1 = (fst$head$tail$blck level) 
          y1 = (snd$head$tail$blck level)

move East level =
    if (gameState level) == 0
        then if(length (blck level) == 2)
                then if(x1 == x)
                        then (activate (getCellAt level (x,(maxi y y1)+1)) (Level (mat level) [(x,(maxi y y1)+1)] ((dpt level) + 1)))
                        else (activate (getCellAt level (x1,y1+1)) (activate (getCellAt level (x,y+1)) (Level (mat level) [(x,y+1), (x1,y1+1)] ((dpt level) + 1)) ))
                else  (activate (getCellAt level (x,y+1)) (activate (getCellAt level (x,y+2)) (Level (mat level) [(x,y+1) , (x,y+2)] ((dpt level) + 1))))
        else level     

    where x = (fst$head$blck level) 
          y = (snd$head$blck level)   
          x1 = (fst$head$tail$blck level) 
          y1 = (snd$head$tail$blck level)  
{-
    *** TODO ***

    Va returna True dacă jocul nu este nici câștigat, nici pierdut.
    Este folosită în cadrul Interactive.
-}

continueGame :: Level -> Bool
continueGame lvl = (gameState lvl == 0)

{-
    *** TODO ***

    Instanțiați clasa `ProblemState` pentru jocul nostru. 
  
    Hint: Un level câștigat nu are succesori! 
    De asemenea, puteți ignora succesorii care 
    duc la pierderea unui level.
-}

instance ProblemState Level Directions where
    successors level = a ++ b ++ c ++ d
                where  a = if gameState (move North level) == 0 then [(North, move North level)] else []    
                       b = if gameState (move South level) == 0 then [(South, move South level)] else [] 
                       c = if gameState (move East level)  == 0 then [(East, move East level)]   else [] 
                       d = if gameState (move West level)  == 0 then [(West, move West level)]   else [] 

    isGoal level = gameState level == 1

    -- Doar petru BONUS
    -- heuristic = undefined
-- empty = emptyLevel (4, 4) (2, 2)
-- onlyHard = foldr (\x y-> addTile 'H' x y) empty [ (x, y) | x <- [0..4], y <- [0..4]]
-- withWinningTile = addTile 'W' (2, 3) onlyHard
