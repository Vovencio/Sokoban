using UnityEngine;
using UnityEngine.UI;
using System.IO;

public class SaveManager : MonoBehaviour
{
    public LevelEditor editor;

    public static SaveManager Instance;

    public Text consoleOutputText;
    public InputField consoleInputField;
    public Text header;

    public Text dif;
    public Text mov;

    // Your level data arrays
    public int[][] levels = new int[96][];
    public int[] playerPositions = new int[96];
    public int[] lDifficult = new int[96];
    public int[] lGoalMoves = new int[96];

    int currentLevelIndex;

    void Start()
    {
        Instance = this;
        InitializeLevels();
        consoleOutputText.text = "Available commands: 'load x' or 'l x'; 'save' or 's'; 'difficulty x' or 'd x'; 'moves x' or 'm x'.";
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Return))
        {
            ExecuteCommand();
        }
    }

    void InitializeLevels()
    {
        for (int i = 0; i < 96; i++)
        {
            levels[i] = new int[128]; // Replace with your level size
            playerPositions[i] = 0;
            lDifficult[i] = 1;
            lGoalMoves[i] = 10;
        }

        LoadFromFile("Assets/SaveData/levelsData.txt");

        LoadLevel(0);
    }

    public void LoadLevel(int levelIndex)
    {
        if (levelIndex >= 0 && levelIndex < 96)
        {
            currentLevelIndex = levelIndex;

            editor.LoadLevel(levels[levelIndex], playerPositions[levelIndex], lDifficult[levelIndex], lGoalMoves[levelIndex]);
            header.text = "Level " + levelIndex;
            dif.text = "dif:\n" + lDifficult[levelIndex];
            mov.text = "mov:\n" + lGoalMoves[levelIndex];

            PrintToConsole("Loaded level " + levelIndex);
        }
        else
        {
            PrintToConsole("Invalid level index.");
        }
    }

    public void SaveLevel()
    {
        levels[currentLevelIndex] = editor.levelData;
        playerPositions[currentLevelIndex] = editor.playerpos;
        lDifficult[currentLevelIndex] = editor.difficulty;
        lGoalMoves[currentLevelIndex] = editor.goalMoves;

        SaveToFile("Assets/SaveData/levelsData.txt");

        SaveToC("Assets/SaveData/levels.h");

        PrintToConsole("Saved current level data for level " + currentLevelIndex);
    }

    void PrintToConsole(string message)
    {
        consoleOutputText.text = message;
        consoleInputField.text = ""; // Clear the input field
    }

    void ExecuteCommand()
    {
        string command = consoleInputField.text;
        string[] parts = command.Split(' ');

        if (parts.Length > 0)
        {
            switch (parts[0].ToLower())
            {
                case "load":
                    LoadLevelCommand(parts);
                    break;
                case "l":
                    LoadLevelCommand(parts);
                    break;
                case "save":
                    SaveLevelCommand();
                    break;
                case "s":
                    SaveLevelCommand();
                    break;
                case "difficulty":
                    SetDifficultyCommand(parts);
                    break;
                case "d":
                    SetDifficultyCommand(parts);
                    break;
                case "moves":
                    SetMovesCommand(parts);
                    break;
                case "m":
                    SetMovesCommand(parts);
                    break;
                default:
                    PrintToConsole("Unknown command.");
                    break;
            }
        }
    }

    void SetDifficultyCommand(string[] parts)
    {
        if (parts.Length > 1)
        {
            if (int.TryParse(parts[1], out int difficulty))
            {
                SetDifficulty(difficulty);
            }
            else
            {
                PrintToConsole("Invalid difficulty value. Use 'difficulty x' where x is an integer.");
            }
        }
        else
        {
            PrintToConsole("Missing difficulty value. Use 'difficulty x' where x is an integer.");
        }
    }

    void SetMovesCommand(string[] parts)
    {
        if (parts.Length > 1)
        {
            if (int.TryParse(parts[1], out int moves))
            {
                SetMoves(moves);
            }
            else
            {
                PrintToConsole("Invalid moves value. Use 'moves x' where x is an integer.");
            }
        }
        else
        {
            PrintToConsole("Missing moves value. Use 'moves x' where x is an integer.");
        }
    }

    void SetDifficulty(int difficulty)
    {
        // Set the difficulty for the current level
        lDifficult[currentLevelIndex] = difficulty;

        dif.text = "dif:\n" + lDifficult[currentLevelIndex];

        editor.difficulty = difficulty;

        PrintToConsole("Set difficulty to " + difficulty + " for level " + currentLevelIndex);
    }

    void SetMoves(int moves)
    {
        // Set the moves for the current level
        lGoalMoves[currentLevelIndex] = moves;

        mov.text = "mov:\n" + lGoalMoves[currentLevelIndex];

        editor.goalMoves = moves;

        PrintToConsole("Set goal moves to " + moves + " for level " + currentLevelIndex);
    }

    void LoadLevelCommand(string[] parts)
    {
        if (parts.Length > 1)
        {
            if (int.TryParse(parts[1], out int levelIndex))
            {
                LoadLevel(levelIndex);
            }
            else
            {
                PrintToConsole("Invalid level index. Use 'load x' where x is a valid level index.");
            }
        }
        else
        {
            PrintToConsole("Missing level index. Use 'load x' where x is a valid level index.");
        }
    }

    void SaveLevelCommand()
    {
        SaveLevel();
    }

    public void SaveToFile(string filePath)
    {
        using (StreamWriter writer = new StreamWriter(filePath))
        {
            // Save levels
            for (int i = 0; i < 96; i++)
            {
                writer.WriteLine(string.Join(",", levels[i]));
            }

            // Save player positions
            writer.WriteLine(string.Join(",", playerPositions));

            // Save difficulties
            writer.WriteLine(string.Join(",", lDifficult));

            // Save goal moves
            writer.WriteLine(string.Join(",", lGoalMoves));
        }

        PrintToConsole("Saved data to file: " + filePath);
    }

    public void LoadFromFile(string filePath)
    {
        if (File.Exists(filePath))
        {
            using (StreamReader reader = new StreamReader(filePath))
            {
                // Load levels
                for (int i = 0; i < 96; i++)
                {
                    string[] levelData = reader.ReadLine().Split(',');
                    for (int j = 0; j < 128; j++)
                    {
                        levels[i][j] = int.Parse(levelData[j]);
                    }
                }

                // Load player positions
                string[] playerPosData = reader.ReadLine().Split(',');
                for (int i = 0; i < 96; i++)
                {
                    playerPositions[i] = int.Parse(playerPosData[i]);
                }

                // Load difficulties
                string[] diffData = reader.ReadLine().Split(',');
                for (int i = 0; i < 96; i++)
                {
                    lDifficult[i] = int.Parse(diffData[i]);
                }

                // Load goal moves
                string[] movesData = reader.ReadLine().Split(',');
                for (int i = 0; i < 96; i++)
                {
                    lGoalMoves[i] = int.Parse(movesData[i]);
                }
            }

            PrintToConsole("Loaded data from file: " + filePath);
        }
        else
        {
            PrintToConsole("File not found: " + filePath);
        }
    }


    public void SaveToC(string filePath)
    {
        using (StreamWriter writer = new StreamWriter(filePath))
        {
            // Save lDifficult array
            SaveArrayToC(writer, "lDifficult", lDifficult);

            // Save lGoalMoves array
            SaveArrayToC(writer, "lGoalMoves", lGoalMoves);

            // Save level data arrays
            for (int i = 0; i < 96; i++)
            {
                SaveArrayToC(writer, "level" + i, levels[i]);
                SaveSingleValueToC(writer, "level" + i + "PlayerPos", playerPositions[i]);
            }
        }

        // PrintToConsole("Saved data to C file: " + filePath);
    }

    void SaveArrayToC(StreamWriter writer, string arrayName, int[] array)
    {
        writer.Write("const int " + arrayName + "[96] = {");
        writer.Write(string.Join(", ", array));
        writer.WriteLine("};");
    }

    void SaveSingleValueToC(StreamWriter writer, string variableName, int value)
    {
        writer.WriteLine("const int " + variableName + " = " + value + ";");
    }

}
