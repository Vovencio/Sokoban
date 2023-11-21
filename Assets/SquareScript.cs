using UnityEngine;
using UnityEngine.UI;

public class SquareScript : MonoBehaviour
{
    public int squareIndex;
    public LevelEditor levelEditor;

    // Attach this method to the button click event in the prefab

    private void OnMouseDown()
    {
        levelEditor.UpdateSquare(squareIndex);
    }
}
