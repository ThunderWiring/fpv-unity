using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class MainMenu : MonoBehaviour
{
    [SerializeField] private Button recordButtn;
    [SerializeField] private TextMeshProUGUI recordButtonText;

    public void startRecord() {
       SceneManager.LoadScene(1);

    }


}
