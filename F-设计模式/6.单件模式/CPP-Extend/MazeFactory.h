class MazeFactory {
public:
    static MazeFactory* instance();
protected:
    mazeFactory() {};
    mazeFactory(int type);
private:
    int _type;
    static MazeFactory* _instance;
}