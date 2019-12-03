#include "basicRender.hpp"
#include<vector>
#include<cmath>
#include<iostream>
#include<string>
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::to_string;
using glm::vec2;
using glm::vec3;


//NOTE:: all shapes are drawn counter-clockwise, so keep that in mind
//The verticies are drawn using indexed rendering, each new primitive (triangles and squares for now) handles its own indicies
BasicRenderer::Vertex  make_vertex(glm::vec2 point,glm::vec3 color){
  BasicRenderer::Vertex vertex = {point,color};
  return vertex; 

}
//uint16_t get_max_index(vector<uint16_t> indicies){
//    uint16_t max = 0;
//    for(size_t i=0;i<indicies.size();i++){
//      if(indicies[i]>max) max = indicies[i];
//    }
//  return max;
//}

//adds a triangle to the vertex and index buffers provided
void add_triangle(glm::vec2 x1, glm::vec2 x2, glm::vec2 x3, glm::vec3 c,vector<BasicRenderer::Vertex>& vertexBuffer,vector<uint16_t>& indicies){
        size_t offset = vertexBuffer.size(); 
    //first, create verticies for each point, with color c
       vertexBuffer.push_back(make_vertex(x1,c));
       vertexBuffer.push_back(make_vertex(x2,c)); 
       vertexBuffer.push_back(make_vertex(x3,c)); 
       indicies.push_back(offset);
       indicies.push_back(offset+1);
       indicies.push_back(offset+2);
  }
//accepts a point in screen space,used as the bottom left of the square with origin at the center of the screen, bounds +1 to -1 
void add_square(glm::vec2 bottom_left,float length, glm::vec3 c, vector<BasicRenderer::Vertex>& vertexBuffer, vector<uint16_t>& indicies){
        size_t offset = vertexBuffer.size();//get the size off the vertex buffer, this gives us the correct starting index
        //create the other three points
      glm::vec2 bottom_right {bottom_left.x+length,bottom_left.y};
      glm::vec2 top_right {bottom_left.x+length,bottom_left.y+length};
      glm::vec2  top_left {bottom_left.x,bottom_left.y+length};
      //push them to the vectors
       vertexBuffer.push_back(make_vertex(bottom_left,c));
       vertexBuffer.push_back(make_vertex(bottom_right,c));
       vertexBuffer.push_back(make_vertex(top_right,c));
       vertexBuffer.push_back(make_vertex(top_left,c));
       // populate indicies in counter_clockwise fashion 
       // first triangle
       indicies.push_back(offset+0);
       indicies.push_back(offset+1);
       indicies.push_back(offset+2);
        //second triangle
       indicies.push_back(offset+2);
       indicies.push_back(offset+3);
       indicies.push_back(offset+0);
}
string string_point(glm::vec2 point){
  return "("+to_string(point.x)+","+to_string(point.y)+")";
}

glm::vec2 get_unit_norm(glm::vec2 vec){
  float x = vec.x;
  float y = vec.y;
  float c, x2,y2;
  if(x!=0){
  c = pow((y/x),2); // c = 1-(x2/x1)^2) 
  x2 = sqrt(c/1+c);
  y2 = -sqrt( 1/c )*x2;
} else{
  c=0;
  x2 = 1;
  y2 = 0;
}
  glm::vec2 out(x2,y2);
  return out;
}
//draws a body centered rectangle from start to end
void add_line(glm::vec2 start, glm::vec2  end,float thickness, glm::vec3 c,vector<BasicRenderer::Vertex>& vertexBuffer, vector<uint16_t>& indicies){  
    vec2 path = end-start;
    cout<<"path"<<string_point(path)<<endl;
    vec2 orth = get_unit_norm(path)*(thickness/2);//get a vector orthogonal to the center line path
    cout<<"norm"<<string_point(orth)<<endl;

    vec2 p0 = start -orth;
    vec2 p1 = start +orth;
    vec2 p2 = end -orth;
    vec2 p3 = end +orth;
    size_t offset = vertexBuffer.size(); 
    vertexBuffer.push_back(make_vertex(p0,c));
    vertexBuffer.push_back(make_vertex(p1,c));
    vertexBuffer.push_back(make_vertex(p2,c));
    vertexBuffer.push_back(make_vertex(p3,c));
    
    //debugging prints
    cout<<"p0"<<string_point(p0)<<endl;
    cout<<"p1"<<string_point(p1)<<endl;
    cout<<"p2"<<string_point(p2)<<endl;
    cout<<"p3"<<string_point(p3)<<endl;




    indicies.push_back(offset+0);
    indicies.push_back(offset+1); 
    indicies.push_back(offset+2); 

    indicies.push_back(offset+2); 
    indicies.push_back(offset+3);
    indicies.push_back(offset+0);
  }


int main(){
  std::vector<BasicRenderer::Vertex> verticies;
  std::vector<uint16_t> indicies;
  glm::vec3 red(1.0,0.0,0.0);
  glm::vec3 green(0.0,1.0,0.0);
  glm::vec2 bottomLeft(0,0);
  add_square(bottomLeft,.1,red,verticies,indicies);



BasicRenderer renderer(verticies,indicies);

renderer.initialize();
GLFWwindow* window = renderer.getWindow();
size_t counter = 1;
float length = 1;
while(!glfwWindowShouldClose(window)){
  glfwPollEvents();
  if(counter%300==0){
    length+=1;
    length = length>=10? 1 : length;
    verticies.clear();
    indicies.clear();
    add_square(bottomLeft,length/10,green,verticies,indicies);
    renderer.update(verticies,indicies); 
  }
  counter++;


  renderer.draw();
}

renderer.shutdown();

}
