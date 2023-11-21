using UnityEngine;

public class LevelEditor : MonoBehaviour
{
    public int[] levelData = new int[128];
    public int playerpos;
    public Sprite[] sprites; // Array of sprites for different states
    public GameObject squarePrefab;
    public SpriteRenderer indicator;
    public float scale;
    public Transform Player;
    int currentObj;
    public int difficulty;
    public int goalMoves;

    void Awake()
    {
        // Create the grid
        CreateGrid();

    }

    private void Update()
    {
        ChangeCurrentObjOnKeyPress();
    }

    void ChangeCurrentObjOnKeyPress()
    {
        for (int i = 0; i < 7; i++)
        {
            // Check if the number keys 1-9 are pressed
            if (Input.GetKeyDown(KeyCode.Alpha1 + i))
            {
                currentObj = i; // Subtract 1 to make it 0-based
                indicator.sprite = sprites[currentObj];
                return;
            }
        }
    }

    void CreateGrid()
    {
        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 16; x++)
            {
                int index = x + y * 16;

                // Instantiate the prefab
                GameObject squareGO = Instantiate(squarePrefab, new Vector3(x * scale, y * scale, 0f), Quaternion.identity);

                // Set the square index and reference to LevelEditor
                SquareScript squareScript = squareGO.GetComponent<SquareScript>();
                squareScript.squareIndex = index;
                squareScript.levelEditor = this;

                squareGO.name = "Square_" + index;

                // Set the initial sprite
                squareGO.GetComponent<SpriteRenderer>().sprite = sprites[levelData[index] % sprites.Length];
                squareGO.GetComponent<SpriteRenderer>().sortingOrder = -index;
            }
        }

        GameObject squarePlayer = GameObject.Find("Square_0");
        Player.GetComponent<SpriteRenderer>().sortingOrder = squarePlayer.GetComponent<SpriteRenderer>().sortingOrder + 1;
        Player.position = squarePlayer.transform.position;
    }

    public void UpdateSquare(int index)
    {
        if (currentObj != 6)
        {
            // Add 1 to the levelData array and update the sprite
            levelData[index] = currentObj;

            // Update the sprite based on the value in levelData
            GameObject squareGO = GameObject.Find("Square_" + index); // Assumes you named the prefab "Square"
            squareGO.GetComponent<SpriteRenderer>().sprite = sprites[levelData[index]];
        }

        else
        {
            playerpos = index;
            GameObject squareGO = GameObject.Find("Square_" + index); // Assumes you named the prefab "Square"
            Player.position = squareGO.transform.position;
            Player.GetComponent<SpriteRenderer>().sortingOrder = squareGO.GetComponent<SpriteRenderer>().sortingOrder + 1;
        }
    }

    public void LoadLevel(int[] newLevelData, int PlayerPos, int Difficulty, int GoalMoves)
    {
        difficulty = Difficulty;
        goalMoves = GoalMoves;

        playerpos = PlayerPos;
        GameObject squarePlayer = GameObject.Find("Square_" + PlayerPos); // Assumes you named the prefab "Square"
        Player.position = squarePlayer.transform.position;
        Player.GetComponent<SpriteRenderer>().sortingOrder = squarePlayer.GetComponent<SpriteRenderer>().sortingOrder + 1;
        if (newLevelData.Length == levelData.Length)
        {
            levelData = newLevelData;

            // Update the visual representation of the grid based on the loaded data
            for (int i = 0; i < levelData.Length; i++)
            {
                GameObject squareGO = GameObject.Find("Square_" + i);
                squareGO.GetComponent<SpriteRenderer>().sprite = sprites[levelData[i] % sprites.Length];
            }
        }
        else
        {
            Debug.LogError("Invalid level data length. Cannot load the level.");
        }
    }
}
