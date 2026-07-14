# Review implementation plan for prisma schema
_Exported on 7/21/2025 at 07:58:47 MDT from Cursor (1.2.2)_

---

**User**

Can you look over the "Implenetation Plan" document, and see if we're missing anything from this prisma schema?

---

**Cursor**

I'll analyze the Implementation Plan against the Prisma schema to identify any missing elements.

After carefully comparing the Implementation Plan with the Prisma schema, I found one **major gap** and a couple of **minor considerations**:

## Major Missing Component:

### **Vote/Like Tracking Model**
The Implementation Plan explicitly mentions:
- "Tindr-style voting" system
- "mutual likes" for matching algorithm  
- "summary/finalize" voting page
- "vote" actions in routes

However, the schema has **no Vote or Like tracking model**. Without this, you can't:
- Track individual artist votes on artworks
- Implement the "mutual likes" matching algorithm
- Show voting progress or allow vote changes
- Create the "summary/finalize" page mentioned in the plan

**Suggested addition:**
```prisma
model Vote {
  id        String   @id @default(uuid())
  artist    Artist   @relation(fields: [artistId], references: [id])
  artistId  String
  artwork   Artwork  @relation(fields: [artworkId], references: [id])
  artworkId String
  event     Event    @relation(fields: [eventId], references: [id])
  eventId   String
  liked     Boolean  // true for like, false for pass
  createdAt DateTime @default(now())

  @@unique([artistId, artworkId, eventId]) // One vote per artist per artwork per event
}
```

## Minor Considerations:

### **Admin Role Granularity**
The plan mentions both "admin" and "super-admin" roles, but `GroupMembership` only has a boolean `isAdmin` field. You might need more granular permissions, or this could be handled at the application level.

### **Session Management**
The plan mentions "session management" but there's no `Session` model. This could be intentional if using JWT tokens or similar stateless authentication.

## Everything Else Looks Good ✅

The schema properly covers:
- All entities mentioned (Artist, Group, Event, Artwork, Match, Invitation, Notification)
- Authentication (password hashing, reset tokens)
- Event phases and artwork status enums
- Image storage (JSON array for file paths)
- Invitation system with tokens
- Notification system with proper types
- Flagging system via artwork status

The **Vote tracking model** is definitely the most critical missing piece for implementing the core voting functionality described in the plan.

```plaintext
model Vote {
  id        String   @id @default(uuid())
  artist    Artist   @relation(fields: [artistId], references: [id])
  artistId  String
  artwork   Artwork  @relation(fields: [artworkId], references: [id])
  artworkId String
  event     Event    @relation(fields: [eventId], references: [id])
  eventId   String
  liked     Boolean  // true for like, false for pass
  createdAt DateTime @default(now())

  @@unique([artistId, artworkId, eventId]) // One vote per artist per artwork per event
}
```

---

**User**

can you make any suggested changes?

---

**Cursor**

I'll add the missing Vote model to your Prisma schema and make a few other improvements based on the implementation plan.

```plaintext
// ... existing code ...

model Artist {
    id            String            @id @default(uuid())
    email         String            @unique
    passwordHash  String
    fullName      String
    profilePic    String? // URL or file path
    memberships   GroupMembership[]
    artworks      Artwork[]
    notifications Notification[]
    votes         Vote[]
    createdAt     DateTime          @default(now())
    resetToken   String? // for password reset
    resetTokenExpiry DateTime?
}

// ... existing code ...

model Event {
    id              String    @id @default(uuid())
    group           Group     @relation(fields: [groupId], references: [id])
    groupId         String
    phase           String // open, voting, closed, archived
    submissionLimit Int
    artworks        Artwork[]
    matches         Match[]
    votes           Vote[]
    createdAt       DateTime  @default(now())
}

model Artwork {
    id          String   @id @default(uuid())
    artist      Artist   @relation(fields: [artistId], references: [id])
    artistId    String
    event       Event?   @relation(fields: [eventId], references: [id])
    eventId     String?
    title       String
    description String?
    medium      String?
    dimensions  String?
    year        Int?
    images      Json // Array of file paths or URLs
    status      ArtworkStatus
    votes       Vote[]
    createdAt   DateTime @default(now())
    Match1      Match?   @relation("MatchArtwork1")
    Match2      Match?   @relation("MatchArtwork2")
}

// ... existing code ...

model Vote {
    id        String   @id @default(uuid())
    artist    Artist   @relation(fields: [artistId], references: [id])
    artistId  String
    artwork   Artwork  @relation(fields: [artworkId], references: [id])
    artworkId String
    event     Event    @relation(fields: [eventId], references: [id])
    eventId   String
    liked     Boolean  // true for like, false for pass
    createdAt DateTime @default(now())

    @@unique([artistId, artworkId, eventId]) // One vote per artist per artwork per event
}

// ... existing code ...
```

Now let me add a couple more enums to make the schema more robust, as mentioned in the implementation plan:

```plaintext
enum ArtworkStatus {
  PORTFOLIO
  EVENT
  MATCHED
  FLAGGED
}

enum NotificationType {
  MATCH
  EVENT
  FLAG
  OTHER
}

enum InvitationStatus {
  PENDING
  ACCEPTED
  EXPIRED
}

enum EventPhase {
  OPEN
  VOTING
  CLOSED
  ARCHIVED
}

enum MatchStatus {
  PENDING
  COMPLETED
  CANCELLED
}

// ... existing code ...
```

```plaintext
model Event {
    id              String     @id @default(uuid())
    group           Group      @relation(fields: [groupId], references: [id])
    groupId         String
    phase           EventPhase // open, voting, closed, archived
    submissionLimit Int
    artworks        Artwork[]
    matches         Match[]
    votes           Vote[]
    createdAt       DateTime   @default(now())
}

// ... existing code ...
```

```plaintext
model Match {
    id         String      @id @default(uuid())
    event      Event       @relation(fields: [eventId], references: [id])
    eventId    String
    artwork1   Artwork?    @relation("MatchArtwork1", fields: [artwork1Id], references: [id])
    artwork1Id String      @unique
    artwork2   Artwork?    @relation("MatchArtwork2", fields: [artwork2Id], references: [id])
    artwork2Id String      @unique
    status     MatchStatus // pending, completed, cancelled
    createdAt  DateTime    @default(now())
}

// ... existing code ...
```

Perfect! I've made the key improvements to your Prisma schema. Here's what I added:

## ✅ **Major Addition: Vote Model**
```prisma
model Vote {
    id        String   @id @default(uuid())
    artist    Artist   @relation(fields: [artistId], references: [id])
    artistId  String
    artwork   Artwork  @relation(fields: [artworkId], references: [id])
    artworkId String
    event     Event    @relation(fields: [eventId], references: [id])
    eventId   String
    liked     Boolean  // true for like, false for pass
    createdAt DateTime @default(now())

    @@unique([artistId, artworkId, eventId]) // One vote per artist per artwork per event
}
```

## ✅ **Type Safety Improvements**
Added two new enums:
- **`EventPhase`**: `OPEN | VOTING | CLOSED | ARCHIVED` (replaces string field)
- **`MatchStatus`**: `PENDING | COMPLETED | CANCELLED` (replaces string field)

## ✅ **Relations Updated**
- Added `votes Vote[]` relations to `Artist`, `Event`, and `Artwork` models
- Updated `Event.phase` to use `EventPhase` enum
- Updated `Match.status` to use `MatchStatus` enum

## **What This Enables:**
- ✅ Tindr-style voting system
- ✅ "Mutual likes" matching algorithm  
- ✅ Vote tracking and summary/finalize pages
- ✅ Proper event phase transitions
- ✅ Better type safety across the application
- ✅ Prevention of duplicate votes per artist/artwork/event

Your schema now fully supports all the voting and matching functionality described in the Implementation Plan! You'll need to run `npx prisma migrate dev --name add-voting-system` to apply these changes to your database.

```plaintext
model Vote {
    id        String   @id @default(uuid())
    artist    Artist   @relation(fields: [artistId], references: [id])
    artistId  String
    artwork   Artwork  @relation(fields: [artworkId], references: [id])
    artworkId String
    event     Event    @relation(fields: [eventId], references: [id])
    eventId   String
    liked     Boolean  // true for like, false for pass
    createdAt DateTime @default(now())

    @@unique([artistId, artworkId, eventId]) // One vote per artist per artwork per event
}
```

