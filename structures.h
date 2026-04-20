
typedef enum{
    NONE,
    PLIERS,
    SPACER 
}Tool;

typedef struct{
    Tool i;
    int dirt;
}ToolCondition;

typedef struct {
    int gloves;
    int glovesDirt;
} Gloves;

typedef struct{
    int x;
    int y;
} Position;

typedef struct{
    Position p;
    Gloves g;
    ToolCondition hand;
}Dentist;

typedef struct {
    int chair;
    int patience;
    /*Pathology ToolCondition[2]; (pathologies a rajouter dans un fichier à part)*/
    int treated;
} Patient;

typedef enum {
    FLOOR,
    CHAIR,
    INSTRUMENTS,
    TRASH,
    EMPTY
} Box;
