<div align="left">

### 42_cursus_irc![---------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

<div align="left">

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)

<nav>
    
### Summary![---------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

<ul>
    <li><a href="#introduction">Introduction</a></li>
    <li><a href="#mandatory-part">Mandatory Part</a></li>
    <li><a href="#bonus-part">Bonus Part</a></li>
    <li><a href="#code-overview">Code Overview</a></li>
    <li><a href="#execution-breakdown">Execution Breakdown</a></li>
    <li><a href="#conclusion">Conclusion</a></li>
    <li><a href="#thanks">Thanks</a></li>
</ul>
</nav>

### Introduction![---------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)
<section id="introduction">

<div align="justify">

The IRC project is about creating a simple IRC server that can handle multiple clients, channels, and basic IRC commands. This project helps in understanding network programming, socket handling, and the IRC protocol by implementing a functional IRC server.

<div align="left">

### Mandatory Part![---------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)
<section id="mandatory-part">

<p>Your IRC server must implement the following features:</p>
<ul>
    <li>Handle multiple clients simultaneously.</li>
    <li>Support basic IRC commands: <code>NICK</code>, <code>USER</code>, <code>JOIN</code>, <code>PART</code>, <code>PRIVMSG</code>, <code>QUIT</code>.</li>
    <li>Manage multiple channels and allow clients to join and leave channels.</li>
    <li>Broadcast messages to all clients in a channel.</li>
    <li>Handle private messages between clients.</li>
</ul>
</section>

<div align="left">

### Bonus Part![---------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)
<section id="bonus-part">

<p>Your IRC server must implement the following features:</p>
<ul>
    <li>Implement a bot that responds to specific commands (e.g., <code>!hello</code>).</li>
    <li>Support file transfers using the DCC protocol.</li>
</ul>
</section>

<div align="left">

### Code Overview![---------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)
<section id="code-overview">

<h3>Server</h3>
<p>The server part of the IRC project includes the following files:</p>
<ul>
    <li><code>server.cpp</code>: Main server logic, including client handling and command processing.</li>
    <li><code>serverSocket.cpp</code>: Functions for setting up and managing sockets and epoll events.</li>
    <li><code>serverCommand.cpp</code>: Functions for handling IRC commands.</li>
</ul>

<h3>Client</h3>
<p>The client part of the IRC project includes the following files:</p>
<ul>
    <li><code>client.cpp</code>: Client class implementation, including command handling and message sending.</li>
    <li><code>clientCommand.cpp</code>: Functions for handling client-specific commands.</li>
</ul>

<h3>Channel</h3>
<p>The channel part of the IRC project includes the following files:</p>
<ul>
    <li><code>channel.cpp</code>: Channel class implementation, including client management and message broadcasting.</li>
</ul>

<h3>Bot</h3>
<p>The bot part of the IRC project includes the following files:</p>
<ul>
    <li><code>bot.cpp</code>: Bot class implementation, including command handling and message responding.</li>
</ul>
</section>

<div align="left">

### Execution Breakdown![---------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)
<section id="execution-breakdown">

<p>For this project, we divided the execution into several main parts:</p>
<ol>
    <li>
        <strong>Server Setup:</strong>
        <p>Setting up the server involved configuring sockets, managing epoll events, and handling client connections. This part required a solid understanding of network programming and socket handling.</p>
    </li>
    <li>
        <strong>Command Handling:</strong>
        <p>Implementing the IRC commands required careful parsing and processing of client messages. We implemented commands such as <code>NICK</code>, <code>USER</code>, <code>JOIN</code>, <code>PART</code>, <code>PRIVMSG</code>, and <code>QUIT</code>.</p>
    </li>
    <li>
        <strong>Channel Management:</strong>
        <p>Managing channels involved handling client joins and leaves, broadcasting messages to all clients in a channel, and maintaining a list of active channels.</p>
    </li>
    <li>
        <strong>Bot Implementation:</strong>
        <p>We implemented a simple bot that responds to specific commands such as <code>!hello</code>. The bot was designed to join channels and interact with clients based on predefined commands.</p>
    </li>
    <li>
        <strong>File Transfer:</strong>
        <p>Implementing file transfer using the DCC protocol was a challenging task. It required setting up a separate socket for the file transfer and ensuring reliable data transmission between clients.</p>
    </li>
</ol>
</section>

<div align="left">

### Conclusion![---------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)
<section id="conclusion">

<p>Overall, the IRC project was a valuable learning experience that provided a deep understanding of network programming and the IRC protocol. It was a challenging project that required careful planning, thorough testing, and effective collaboration. The skills and knowledge gained from this project will be invaluable in future endeavors.</p>
</section>

<div align="left">

### Thanks![---------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)
<section id="thanks">

<p>Thanks to my project partner for their collaboration and support throughout this project. Their expertise and dedication were instrumental in the successful completion of the IRC server.</p>

</div>
