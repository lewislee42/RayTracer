
<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->



<!-- PROJECT LOGO -->
<br />
<div align="center">

<h3 align="center">RayTracer</h3>

  <p align="center">
    A Raytracing project that is meant for me to learn the modern day implementation of Raytracing
    <br />
    <br />
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

<img src="https://github.com/user-attachments/assets/1fa112d1-b85f-476e-8f33-34243dc09cb1" />

This project is to implement raytracing(path tracing) using Apples Metal api, currently the plan is to implement what i can find and are interested in but for the moment implementing BVH is the goal,
so far this project features regular path tracing, global illumination (direct + indirect lighting)




### Built With

* [![C++][C++]][C++-url]
* [![Apple][Apple]][Apple-url]
* [![SDL][SDL]][SDL-url]




<!-- GETTING STARTED -->
## Getting Started

For this project, I made it primarily on my daily macbook so the installation and usage are based off running it on macos

### Prerequisites
You'll need to install xcode and `SDL3`


### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/lewislee42/RayTracer.git
   ```
2. Open the project with xcode (click on the xcode project file in the repo)
4. Run and build (at the top left of xcode)



### Usage

<img src="https://github.com/user-attachments/assets/78d5eae3-8e66-40a9-adfc-7ee816545849" />
Just running it out of the box will run the Raytracer with 100 samples per pixel, 10 bounces per ray shot which will look like the image above (but hitting bad amounts of frames)

<br/>

To change that you can head to the main and reduce the sample ray count to something like 10 to achive playable amount of frames, it will look worser just cause of how the sample data
also you can move around the scene with `WASD` and the arrow keys to look around
<br/>
<img width="511" alt="image" src="https://github.com/user-attachments/assets/531515bc-ba8b-41f5-bee2-9a63684dffe0" />
<br/>

[project_video][project-video-1]


To play around with the color of the lights and the color of the Spheres head to the image below and just play around with the values
<br/>
<img width="556" alt="image" src="https://github.com/user-attachments/assets/f16d9a31-ff85-4bc6-82c6-25f33671cacc" />
<br/>

[project_video][project-video-2]

<br/>

<!-- ROADMAP -->
## Roadmap

- [x] Rendering Sphere
- [x] Rendering Sphere with lighting
- [x] Rendering Sphere with material textures
- [x] Move Rendering onto the GPU
- [x] Implement Direct Lighting
- [ ] Implement BVH








<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo_name.svg?style=for-the-badge
[contributors-url]: https://github.com/github_username/repo_name/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/github_username/repo_name.svg?style=for-the-badge
[forks-url]: https://github.com/github_username/repo_name/network/members
[stars-shield]: https://img.shields.io/github/stars/github_username/repo_name.svg?style=for-the-badge
[stars-url]: https://github.com/github_username/repo_name/stargazers
[issues-shield]: https://img.shields.io/github/issues/github_username/repo_name.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo_name/issues
[license-shield]: https://img.shields.io/github/license/github_username/repo_name.svg?style=for-the-badge
[license-url]: https://github.com/github_username/repo_name/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[project-video-1]: https://github.com/user-attachments/assets/cc93355f-1693-441d-98f8-d3664ceb1e42
[project-video-2]: https://github.com/user-attachments/assets/95e8e5f0-83ef-484f-998f-f22a69d080c2
[C++]: https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white
[C++-url]: #
[RayLib]: https://img.shields.io/badge/RAYLIB-FFFFFF?style=for-the-badge&logo=raylib&logoColor=black
[Raylib-url]: https://www.raylib.com
[Apple]: https://img.shields.io/badge/Apple-%23000000.svg?style=for-the-badge&logo=apple&logoColor=white
[Apple-url]: https://developer.apple.com/metal/
[SDL]: https://img.shields.io/badge/SDL-ffffff
[SDL-url]: https://www.libsdl.org








