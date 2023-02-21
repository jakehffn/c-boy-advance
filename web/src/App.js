import { useEffect } from "react";

function App() {  

  return (
    <div className="flex flex-row w-full h-screen justify-center bg-neutral-900">
      <div className="flex flex-col h-full justify-center">
        <canvas id="canvas" height="144" width="160"></canvas>
      </div>
    </div>
  );
}

export default App;
