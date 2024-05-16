# UWB Experiments:

## Experiment results:

So far the following results have been reached:

- Configure two boards as a Tag and an Anchor and measure the distance between the two
    - Without Antenna delay calibration the scaling is off by a lot
- Created an automatic Antenna delay sketch
    - Uses a Tag that is at a known distance form the anchor.
    - uses a binary approximation algorithm to re-configure the antenna delay untill the error goes down to 3cm max
- Created a Processing sketch that communicates to the anchors via Serial and calculates the position of a tag in 2d space. The output is shown below:

[UWB ranging demo.mp4](https://prod-files-secure.s3.us-west-2.amazonaws.com/3c939a49-8303-49f7-b233-f08f2f83ed3d/37767032-bd52-45a7-8085-21f37c324d3b/UWB_ranging_demo.mp4)

## Open issues:

A lot of issues were tackled during testing. Some of them are still unsolved:

- Having multiple achors/tags in the same system caused major interference
    - Lots of negative distances and distances in the hundreds of meters were measured when two anchors are used
        - (visible in the video above, the green dot is the tag position and it simply ignores all non valid distance data).
    - unplugging everything but 1 anchor and 1 tag immediately solves the problem
    - The following things have been tried to mitigate this problem:
        - Made sure that every tag/anchor uses their own unique ID (they all use the ESP’s MAC address as ID)
            - This is something I did from the start, as having two devices with the same ID will definitely cause problems
        - Having all anchors poll at random intervals
    - There are some other people who seemed to have this problem, but no real solution is found
- The UWB system default seems to be built to monitor the location of a lot of tags within an environment, where the tags are the moving parts and the anchors are static. This means that the tags themselves don’t know where they are, but a central computing unit does know where the tags are.
    - If we want the tags to know their own locations, there are two different approaches:
        - Create a system where the anchor/tag system is reversed. This way all wearables will be ahcors that calculate their position based on the distances to the (static) tags.
        - Have a system where the wearables wirelessly receive the their position from the central system
            - Perhaps only in key moments (e.g. when a person enters a different room)
    - We could also just accept the fact that the wearables don’t know their own position.
