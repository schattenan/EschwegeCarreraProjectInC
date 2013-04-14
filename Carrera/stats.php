<?php
 
define ("SEPERATOR", "," );
define ("PATH"     , "stats.csv");
 
class CSVParser
{
    private $mode;
    private $numberOfPlayers;
    private $numberOfRounds;
    private $playername;
    private $roundTime;
    private $rank;
    private $bestRound;
    private $totalTime;
 
    public function getMode() { return $this->mode[0]; }    
    public function getNumberOfPlayers() { return $this->numberOfPlayers[0]; }  
    public function getNumberOfRounds() { return $this->numberOfRounds[0]; }
    public function getPlayername($player) { return $this->playername[$player]; }
    public function getRoundTime($round,$player) { return $this->roundTime[$round][$player]; }
    public function getRank($player) { return $this->rank[$player]; }
    public function getBestRound($player) { return $this->bestRound[$player]; }
    public function getTotalTime($player) { return $this->totalTime[$player]; }
 
    public function initCSV() 
    {
        $datei = fopen( PATH ,"r");
 
        if( !$datei)
        {
            echo "Failure, could not open file PATH";
        }
        else
        {
            $this->mode = fgetcsv ( $datei, 100, SEPERATOR);
            $this->numberOfPlayers = fgetcsv ( $datei, 100, SEPERATOR);
            $this->numberOfRounds = fgetcsv ( $datei, 100, SEPERATOR);
            $this->playername = fgetcsv ( $datei, 100, SEPERATOR);
 
            for($i=0;$i<$this->numberOfRounds[0];$i++)
            {
                $this->roundTime[$i] = fgetcsv ( $datei, 100, SEPERATOR);
            }
 
            $this->rank = fgetcsv ( $datei, 100, SEPERATOR);
            $this->bestRound = fgetcsv ( $datei, 100, SEPERATOR);
            $this->totalTime = fgetcsv ( $datei, 100, SEPERATOR);
 
            $endSignal = fgetcsv ( $datei, 100, SEPERATOR);
 
            if( !($endSignal[0] == "El Psy Congroo") )
            {
                echo "Failure, csv file is broken!";
            }
 
            fclose($datei);
        }
    }  
}
 
?>