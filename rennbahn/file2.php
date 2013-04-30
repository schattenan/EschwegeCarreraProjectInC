<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
       "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<link rel="stylesheet" type="text/css" href="style.css">
<title>Urkunde</title>
</head>
<body>

<?php
 
define ("SEPERATOR", "," );
define ("PATH"     , "stats.csv");
define ("MAXROUNDS", 20 );
 
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
 
            if( !($endSignal[0] == "End") )
            {
                echo "Failure, csv file is broken!";
            }
 
            fclose($datei);
        }
    } 
 
}
 

    
    $test = new CSVParser();
    $test->initCSV();
    
    echo '<div style="float:left;">';
	echo '<div style="width:380px; margin-bottom:10px; font-variant:small-caps;">';
	echo '<div style="padding:4px 0px 10px 0px; color:black; border-bottom:8px dotted #F2591F; letter-spacing:4.5px; text-align:center;"><h1>BG Eschwege Carrera(TM)-Bahn</h1></div>';
	echo '<div style="padding:7px; letter-spacing:8px; text-align:center;"><h2>Urkunde</h2></div>';
	echo '</div>';
	if($test->getMode() == 'Zeitrennen') {
		echo '<div style="border:4px solid #F2591F; width:370px; font-size:32px; font-weight:bold; margin-bottom:20px;"><div style="float:left; background-color:#F2591F; color:white; padding-right:7px;">Rennmodus</div><div style="float:right; background-color:white; padding-right:5px;">' . $test->getMode() . '</div><div style="clear:both;"></div></div>';
	}
    elseif($test->getMode() == 'Rennen') {
		echo '<div style="border:4px solid #F2591F; width:320px; font-size:32px; font-weight:bold; margin-bottom:20px;"><div style="float:left; background-color:#F2591F; color:white; padding-right:7px;">Rennmodus</div><div style="float:right; background-color:white; padding-right:5px;">' . $test->getMode() . '</div><div style="clear:both;"></div></div>';
	}
	elseif($test->getMode() == 'Knock Out') {
		echo '<div style="border:4px solid #F2591F; width:365px; font-size:32px; font-weight:bold; margin-bottom:20px;"><div style="float:left; background-color:#F2591F; color:white; padding-right:7px;">Rennmodus</div><div style="float:right; background-color:white; padding-right:5px;">' . $test->getMode() . '</div><div style="clear:both;"></div></div>';
	}
	echo '</div>';
	echo '<div style="float:right;">';
	echo '<img src="logo.jpg" style="width:280px;">';
    echo '</div><div style="clear:both;"></div>';
	
    ?>
    
    <table>
        <tr>
            <th width= 20%> </th>
            <?php for($i=0;$i < $test->getNumberOfPlayers() ; $i++) { echo "<th  align='center' width=". (80/$test->getNumberOfPlayers())  ."% >" . $test->getPlayername($i) . "</th>"; } ?>
        </tr>
            <?php for($i=0;$i < $test->getNumberOfRounds() && $i< MAXROUNDS ; $i++) 
                { 
                    echo '<tr class="'; if($i % 2 == 0) { echo "a"; } else { echo "b"; } echo "\"> <td style=\"padding-left:5px;\"> Runde " . ($i+1) . "</td>";
                    for($j=0;$j < $test->getNumberOfPlayers() ; $j++) 
                    {
                        echo "<td  align='center'>" . $test->getRoundTime($i,$j) . "</td>" ;
                    }
                    echo "</tr>\n";
                }
            ?>
        <tr style="border-bottom:2px solid #F2591F;">
         <td> </td>
            <?php for($i=0;$i < $test->getNumberOfPlayers() ; $i++) { echo "<td align='center'>&nbsp;</td>"; } ?>
            
        </tr>
    
        <tr class="b">
         <td style="padding-left:5px; min-width:160px;">Beste Rundenzeit</td>
            <?php for($i=0;$i < $test->getNumberOfPlayers() ; $i++) { echo "<td  align='center'>" . $test->getRoundTime($test->getBestRound($i)-1,$i) . "</td>"; } ?>
            
        </tr>
        
        <tr class="a">
         <td style="padding-left:5px;">Gesamtzeit</td>
            <?php for($i=0;$i < $test->getNumberOfPlayers() ; $i++) { echo "<td  align='center'>" . $test->getTotalTime($i) . "</td>"; } ?>
    
        </tr>
        
        <tr class="b" style="font-weight:bold;">
         <td style="padding-left:5px;">Platzierung</td>
            <?php for($i=0;$i < $test->getNumberOfPlayers() ; $i++) { echo "<td style=\"font-size:28px;\"  align='center'>" . $test->getRank($i) . ". </td>"; } ?>
            
        </tr>
        
        
    </table>
    
    <?php
    echo '<div style="margin:0px auto; width:600px; text-align:center; border-bottom:1px solid #F2591F; padding:5px; margin-top:30px; font-size:20px;"> Urkunde wurde generiert am ' . date('d.m.Y') . ' um ' . date('H:i') . '</div>';
    ?>
	
	<div style="text-align:center;">Ein Projekt des Abiturjahrgangs 2013 des Beruflichen Gymnasiums Eschwege</div>
    
    
    
    </body>
</html>
